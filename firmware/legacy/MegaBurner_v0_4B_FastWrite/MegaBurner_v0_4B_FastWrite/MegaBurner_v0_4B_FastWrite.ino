/**
 * Mega Burner - WebSerial v0.4B FastWrite
 * Base: maximaas/MegaBurner / MX29L3211
 *
 * v0.4A: removed reset() before every read block.
 * v0.4B: adds a fast write transaction mode that avoids reset()+delay(500)
 *         after every 4096-byte block. Original safe W command remains available.
 *
 * Protocol v0.4B uses newline-terminated ASCII commands:
 *   C\n                    -> check id, returns 4 ASCII chars, e.g. C2F9
 *   E\n                    -> erase, returns %
 *   Rblock,size\n          -> read, returns size raw bytes
 *   Woffset,page,size\n    -> safe write block, returns &, receives size bytes, writes, returns %, resets after block
 *   S\n                    -> begin fast write transaction, resets once, returns %
 *   Xoffset,page,size\n    -> fast write block, returns &, receives size bytes, writes, returns %, no reset after block
 *   Z\n                    -> finish fast write transaction, resets once, returns %
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
      buffer[idx++] = (byte)Serial.read();
      lastByteAt = millis();
    } else if (millis() - lastByteAt > RX_TIMEOUT_MS) {
      return false;
    }
  }
  return true;
}

void checkChip() {
  mx29l3211.readId();
  mx29l3211.reset();
}

void readBlock(char *param) {
  long v[2];
  if (!parseLongs(param, v, 2)) {
    Serial.write(SERIAL_ACK_ERROR);
    return;
  }
  long block_id = v[0];
  long block_size = v[1];
  if (block_id < 0 || block_size <= 0) {
    Serial.write(SERIAL_ACK_ERROR);
    return;
  }

  // v0.4A/v0.4B FastRead: no reset before each read block.
  mx29l3211.read16(block_id, block_size);
}

void eraseChip() {
  mx29l3211.reset();
  mx29l3211.erase();
  Serial.write(SERIAL_ACK_DONE);
}

void writeBlockCommon(char *param, bool resetAfterBlock) {
  long v[3];
  if (!parseLongs(param, v, 3)) {
    Serial.write(SERIAL_ACK_ERROR);
    return;
  }

  long offset = v[0];
  long page_size = v[1];
  long block_size = v[2];

  if (offset < 0 || page_size <= 0 || block_size <= 0 || block_size > MAX_WRITE_BLOCK) {
    Serial.write(SERIAL_ACK_ERROR);
    return;
  }
  if (block_size < page_size) page_size = block_size;

  Serial.write(SERIAL_ACK_READY);
  if (!receiveExact(writeBuffer, block_size)) {
    Serial.write(SERIAL_ACK_ERROR);
    return;
  }

  mx29l3211.write16(offset, page_size, block_size, writeBuffer);
  Serial.write(SERIAL_ACK_DONE);

  if (resetAfterBlock) {
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

void processCommand(char *cmd) {
  if (!cmd || cmd[0] == '\0') return;

  switch (cmd[0]) {
    case 'C':
      checkChip();
      break;
    case 'R':
      readBlock(cmd + 1);
      break;
    case 'E':
      eraseChip();
      break;
    case 'W':
      // Safe/original write path: reset after every block.
      writeBlockCommon(cmd + 1, true);
      break;
    case 'S':
      // Begin fast write transaction.
      beginFastWrite();
      break;
    case 'X':
      // Fast block write: no reset after every block.
      writeBlockCommon(cmd + 1, false);
      break;
    case 'Z':
      // Finish fast write transaction.
      finishFastWrite();
      break;
    default:
      Serial.write(SERIAL_ACK_ERROR);
      break;
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
      // Command too long; reset parser and signal error.
      commandLen = 0;
      Serial.write(SERIAL_ACK_ERROR);
    }
  }
}
