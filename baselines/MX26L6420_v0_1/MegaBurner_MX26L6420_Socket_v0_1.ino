/**
 * MegaBurner WebSerial MX26L6420 Socket-Compatible v0.1
 *
 * Dedicated firmware for MX26L6420MC-90 SOP44 mounted in an MX29L3211-compatible socket.
 * It is NOT compatible with MX29L3211 because the socket pin meanings are swapped:
 *   MX29L3211 pin 1  = WE   -> MX26L6420 pin 1  = A21  -> Arduino D7 / PH4
 *   MX29L3211 pin 33 = BYTE -> MX26L6420 pin 33 = WE   -> Arduino D6 / PH3
 *
 * Keep the proven MX29L3211 firmware/tool separate.
 */

#include "MX26L6420_Socket.h"
#include <stdlib.h>

#define SERIAL_BAUD 500000
#define CMD_BUFFER_SIZE 80
#define MAX_WRITE_BLOCK 4096
#define RX_TIMEOUT_MS 10000UL

#define ACK_READY '&'
#define ACK_DONE  '%'
#define ACK_ERROR '!'

MX26L6420Socket flash;
static byte writeBuffer[MAX_WRITE_BLOCK];
static char commandBuffer[CMD_BUFFER_SIZE];
static uint8_t commandLen = 0;
static bool idOkLatch = false;

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

static void printHex16(uint16_t v) {
  char buf[5];
  sprintf(buf, "%04X", v);
  Serial.print(buf);
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

void versionInfo() {
  Serial.print("MBW MX26L6420_SOCKET v0.1 ");
  Serial.print("BAUD="); Serial.print(SERIAL_BAUD);
  Serial.print(" SIZE="); Serial.print(MX26L6420Socket::SIZE_BYTES);
  Serial.print(" MODE=WORD16 ");
  Serial.print("PINOUT=MX29L3211_SOCKET_SWAP ");
  Serial.println("A21=PH4_D7 WE=PH3_D6 OE=PH1_D16 CE=PH6_D9");
}

void checkId() {
  uint16_t mfr = 0, dev = 0;
  flash.readId(&mfr, &dev);
  bool ok = ((mfr & 0x00FF) == (MX26L6420Socket::MANUFACTURER_ID & 0x00FF)) &&
            (dev == MX26L6420Socket::DEVICE_ID || (dev & 0x00FF) == (MX26L6420Socket::DEVICE_ID & 0x00FF));
  idOkLatch = ok;

  Serial.print("ID:MFR="); printHex16(mfr);
  Serial.print(",DEV="); printHex16(dev);
  Serial.print(",EXPECTED="); printHex16(MX26L6420Socket::MANUFACTURER_ID);
  Serial.print("/"); printHex16(MX26L6420Socket::DEVICE_ID);
  Serial.print(",OK="); Serial.println(ok ? "1" : "0");
}

void eraseChip() {
  if (!idOkLatch) { Serial.write(ACK_ERROR); Serial.println("I"); return; }
  bool ok = flash.erase();
  Serial.write(ok ? ACK_DONE : ACK_ERROR);
  if (!ok) Serial.println("E");
}

void readBlock(char *param) {
  long v[2];
  if (!parseLongs(param, v, 2)) { Serial.write(ACK_ERROR); Serial.println("C"); return; }
  long blockId = v[0];
  long blockSize = v[1];
  if (blockId < 0 || blockSize <= 0 || (blockSize & 1)) { Serial.write(ACK_ERROR); Serial.println("B"); return; }
  uint32_t offset = (uint32_t)blockId * (uint32_t)blockSize;
  if (offset + (uint32_t)blockSize > MX26L6420Socket::SIZE_BYTES) { Serial.write(ACK_ERROR); Serial.println("O"); return; }
  flash.read16((uint32_t)blockId, (uint32_t)blockSize);
}

void writeBlock(char *param, bool fastMode) {
  if (!idOkLatch) { Serial.write(ACK_ERROR); Serial.println("I"); return; }

  long v[3];
  if (!parseLongs(param, v, 3)) { Serial.write(ACK_ERROR); Serial.println("C"); return; }
  long offset = v[0];
  long pageSize = v[1];
  long blockSize = v[2];

  if (offset < 0 || blockSize <= 0 || blockSize > MAX_WRITE_BLOCK) { Serial.write(ACK_ERROR); Serial.println("B"); return; }
  if ((offset & 1) || (blockSize & 1)) { Serial.write(ACK_ERROR); Serial.println("A"); return; }
  if ((uint32_t)offset + (uint32_t)blockSize > MX26L6420Socket::SIZE_BYTES) { Serial.write(ACK_ERROR); Serial.println("O"); return; }

  // MX26L6420 is a 4M x16 device. This socket-compatible build uses safe 16-bit word program.
  // pageSize=2 is a sentinel meaning 1 word. Page Program 128/256 is intentionally rejected.
  if (pageSize != 2) { Serial.write(ACK_ERROR); Serial.println("G"); return; }

  Serial.write(ACK_READY);
  if (!receiveExact(writeBuffer, blockSize)) { Serial.write(ACK_ERROR); Serial.println("R"); return; }

  bool ok = flash.write16WordBlock((uint32_t)offset, (uint32_t)blockSize, writeBuffer);
  Serial.write(ok ? ACK_DONE : ACK_ERROR);
  if (!ok) Serial.println("P");
  if (!fastMode) flash.reset();
}

void beginFastWrite() {
  if (!idOkLatch) { Serial.write(ACK_ERROR); Serial.println("I"); return; }
  flash.reset();
  Serial.write(ACK_DONE);
}

void finishFastWrite() {
  flash.reset();
  Serial.write(ACK_DONE);
}

void crcFlash(char *param) {
  long v[2];
  if (!parseLongs(param, v, 2)) { Serial.write(ACK_ERROR); Serial.println("C"); return; }
  long offset = v[0];
  long size = v[1];
  if (offset < 0 || size <= 0 || (offset & 1) || (size & 1) ||
      (uint32_t)offset + (uint32_t)size > MX26L6420Socket::SIZE_BYTES) {
    Serial.write(ACK_ERROR); Serial.println("B"); return;
  }
  uint32_t crc = flash.crc32_16((uint32_t)offset, (uint32_t)size);
  char buf[9];
  sprintf(buf, "%08lX", crc);
  Serial.print(buf);
}

void handleCommand(char *cmd) {
  if (!cmd || !cmd[0]) return;
  char op = cmd[0];
  char *param = cmd + 1;

  switch (op) {
    case 'V': versionInfo(); break;
    case 'C': checkId(); break;
    case 'J': checkId(); break;
    case 'E': eraseChip(); break;
    case 'R': readBlock(param); break;
    case 'W': writeBlock(param, false); break;
    case 'S': beginFastWrite(); break;
    case 'X': writeBlock(param, true); break;
    case 'Z': finishFastWrite(); break;
    case 'K': crcFlash(param); break;
    default: Serial.write(ACK_ERROR); Serial.println("C"); break;
  }
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  flash.init();
}

void loop() {
  while (Serial.available()) {
    char c = (char)Serial.read();
    if (c == '\r') continue;
    if (c == '\n') {
      commandBuffer[commandLen] = '\0';
      handleCommand(commandBuffer);
      commandLen = 0;
    } else if (commandLen < CMD_BUFFER_SIZE - 1) {
      commandBuffer[commandLen++] = c;
    } else {
      commandLen = 0;
      Serial.write(ACK_ERROR);
      Serial.println("C");
    }
  }
}
