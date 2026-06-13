/**
 * MegaBurner - WebSerial v0.5E Final500K
 * Base: maximaas/MegaBurner / MX29L3211
 *
 * Consolidated final HTML-port branch at stable 500000 baud:
 *   v0.5B_500K SafeCore: firmware capabilities command + status-aware busy polling.
 *   v0.5C FastCRC Table: CRC32 table in PROGMEM, faster verification without dump.
 *   v0.5D HTML Chunk Queue: handled by the HTML application.
 *   v0.5E Skip FF After Erase: handled by the HTML application using existing X/W commands.
 *
 * Protocol uses newline-terminated ASCII commands:
 *   V\n                    -> version/capabilities line
 *   C\n                    -> check id, returns 4 ASCII chars, e.g. C2F9
 *   E\n                    -> chip erase, returns % or !E/!T
 *   Rblock,size\n          -> read, returns size raw bytes
 *   Woffset,page,size\n    -> safe write block, returns &, receives size bytes, writes, returns % or !P/!E/!T
 *   S\n                    -> begin fast write transaction, returns %
 *   Xoffset,page,size\n    -> fast write block, returns &, receives size bytes, writes, returns % or !P/!E/!T
 *   Z\n                    -> finish fast write transaction, returns %
 *   Koffset,size\n         -> CRC32 over flash byte range, returns 8 ASCII hex chars or !C
 *   Q\n                    -> raw status register, returns 4 ASCII hex chars
 *   L\n                    -> clear status register, returns %
 *
 * Recommended HTML parameters:
 *   baudrate=500000, writeBlock=4096, pageSize=256, readBlock=16384
 */

#include "MX29L3211.h"
#include <stdlib.h>

#define SERIAL_BAUD 500000
#define CMD_BUFFER_SIZE 64
#define MAX_WRITE_BLOCK 4096
#define SERIAL_ACK_READY '&'
#define SERIAL_ACK_DONE  '%'
#define SERIAL_ACK_ERROR '!'
#define RX_TIMEOUT_MS 10000UL

MX29L3211 mx29l3211 = MX29L3211();
static byte writeBuffer[MAX_WRITE_BLOCK];
static char commandBuffer[CMD_BUFFER_SIZE];
static uint8_t commandLen = 0;

static bool parseLongs(char *s, long *values, uint8_t count) {
  for (uint8_t i = 0; i < count; i++) {
    char *endPtr = NULL;
    values[i] = strtol(s, &endPtr, 10);
    if (endPtr == s) return false;
    if (i < count - 1) {
      if (*endPtr != ',') return false;
      s = endPtr + 1;
    }
  }
  return true;
}

static bool receiveExact(byte *buffer, long size) {
  long idx = 0;
  unsigned long lastByteAt = millis();
  while (idx < size) {
    int available = Serial.available();
    if (available > 0) {
      while (available-- > 0 && idx < size) {
        buffer[idx++] = (byte)Serial.read();
      }
      lastByteAt = millis();
    } else if (millis() - lastByteAt > RX_TIMEOUT_MS) {
      return false;
    }
  }
  return true;
}

static char flashResultCode(uint8_t res) {
  switch (res) {
    case MB_FLASH_OK: return 0;
    case MB_FLASH_PROGRAM_FAIL: return 'P';
    case MB_FLASH_ERASE_FAIL: return 'E';
    case MB_FLASH_TIMEOUT: return 'T';
    default: return 'U';
  }
}

static void sendError(char code) {
  Serial.write(SERIAL_ACK_ERROR);
  Serial.write(code);
}

static void sendFlashResult(uint8_t res) {
  if (res == MB_FLASH_OK) Serial.write(SERIAL_ACK_DONE);
  else sendError(flashResultCode(res));
}

static void printHex32(uint32_t value) {
  const char hex[] = "0123456789ABCDEF";
  for (int8_t shift = 28; shift >= 0; shift -= 4) {
    Serial.write(hex[(value >> shift) & 0x0F]);
  }
}

static void printHex16(word value) {
  const char hex[] = "0123456789ABCDEF";
  for (int8_t shift = 12; shift >= 0; shift -= 4) {
    Serial.write(hex[(value >> shift) & 0x0F]);
  }
}

void versionInfo() {
  Serial.println(F("MBW 0.5E Final500K MX29L3211 BAUD=500000 FASTREAD FASTWRITE FASTCRC_TABLE SAFESTATUS SKIPFF_HTML PAGE=256 MAXW=4096 MAXR=32768"));
}

void checkChip() {
  mx29l3211.readId();
  mx29l3211.reset();
}

void readBlock(char *param) {
  long v[2];
  if (!parseLongs(param, v, 2)) { sendError('C'); return; }
  long block_id = v[0];
  long block_size = v[1];
  if (block_id < 0 || block_size <= 0) { sendError('C'); return; }

  // FastRead: no reset before each read block.
  mx29l3211.read16(block_id, block_size);
}

void eraseChip() {
  mx29l3211.reset();
  uint8_t res = mx29l3211.eraseStatus();
  if (res == MB_FLASH_OK) {
    // Return to Read Array mode once after erase. This keeps read/dump sane after erase.
    mx29l3211.reset();
  }
  sendFlashResult(res);
}

void writeBlockCommon(char *param, bool resetAfterBlock) {
  long v[3];
  if (!parseLongs(param, v, 3)) { sendError('C'); return; }

  long offset = v[0];
  long page_size = v[1];
  long block_size = v[2];

  if (offset < 0 || page_size <= 0 || block_size <= 0 || block_size > MAX_WRITE_BLOCK) {
    sendError('B');
    return;
  }
  if ((offset & 1) || (block_size & 1) || (page_size & 1)) {
    sendError('A');
    return;
  }
  if (block_size < page_size) page_size = block_size;

  Serial.write(SERIAL_ACK_READY);
  if (!receiveExact(writeBuffer, block_size)) {
    sendError('R');
    return;
  }

  uint8_t res = mx29l3211.write16Status(offset, page_size, block_size, writeBuffer);
  sendFlashResult(res);

  if (res == MB_FLASH_OK && resetAfterBlock) {
    delay(100);
    mx29l3211.reset();
  }
}

void beginFastWrite() {
  mx29l3211.reset();
  Serial.write(SERIAL_ACK_DONE);
}

void finishFastWrite() {
  delay(100);
  mx29l3211.reset();
  Serial.write(SERIAL_ACK_DONE);
}

void crcFlash(char *param) {
  long v[2];
  if (!parseLongs(param, v, 2)) { sendError('C'); return; }

  long offset = v[0];
  long size = v[1];
  if (offset < 0 || size <= 0) { sendError('C'); return; }
  if ((offset & 1) || (size & 1)) { sendError('A'); return; }

  uint32_t crc = mx29l3211.crc32_16((uint32_t)offset, (uint32_t)size);
  printHex32(crc);
}

void statusReg() {
  word sr = mx29l3211.status();
  printHex16(sr);
}

void clearFlashStatus() {
  mx29l3211.clearStatus();
  Serial.write(SERIAL_ACK_DONE);
}

void processCommand(char *cmd) {
  if (!cmd || cmd[0] == '\0') return;

  switch (cmd[0]) {
    case 'V': versionInfo(); break;
    case 'C': checkChip(); break;
    case 'R': readBlock(cmd + 1); break;
    case 'E': eraseChip(); break;
    case 'W': writeBlockCommon(cmd + 1, true); break;
    case 'S': beginFastWrite(); break;
    case 'X': writeBlockCommon(cmd + 1, false); break;
    case 'Z': finishFastWrite(); break;
    case 'K': crcFlash(cmd + 1); break;
    case 'Q': statusReg(); break;
    case 'L': clearFlashStatus(); break;
    default: sendError('?'); break;
  }
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  mx29l3211.init();
}

void loop() {
  while (Serial.available()) {
    char c = (char)Serial.read();
    if (c == '\r') continue;

    if (c == '\n') {
      commandBuffer[commandLen] = '\0';
      processCommand(commandBuffer);
      commandLen = 0;
      continue;
    }

    if (commandLen < CMD_BUFFER_SIZE - 1) {
      commandBuffer[commandLen++] = c;
    } else {
      commandLen = 0;
      sendError('C');
    }
  }
}
