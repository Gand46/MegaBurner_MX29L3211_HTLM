/**
 * MegaBurner Web Serial MultiChip v0.7A ProfileSafe
 *
 * Supported profiles on the same pin-number-preserving SOP44 socket:
 *   1. MX29L3211        4 MiB, validated Page Program 256/128 bytes
 *   2. MX26L6420        8 MiB, validated Word Program x16, socket pin swap
 *   3. MX29LV160CTMC    2 MiB, Word Program x16, datasheet/logically validated
 *
 * Safety model:
 *   - Firmware boots with no active profile and universal safe pin states.
 *   - HTML must select a profile using a profile-specific confirmation key.
 *   - Erase/write/read/CRC are blocked until the expected manufacturer/device ID matches.
 *   - Profile is locked after a successful ID check and can only be changed after reset/reconnect.
 *   - A write session requires a successful erase in the same session.
 *
 * Protocol (newline-terminated ASCII commands):
 *   V\n                       version/capabilities
 *   P1,3211\n                 select MX29L3211
 *   P2,6420\n                 select MX26L6420 socket-compatible
 *   P3,160C\n                 select MX29LV160CTMC socket-compatible
 *   Q\n                       active profile/status
 *   C\n                       check ID and lock profile on success
 *   E\n                       erase: returns &, then % or !<code>\n
 *   S\n                       begin write session
 *   Woffset,page,size\n       safe block write: &, receives data, then % or !...
 *   Xoffset,page,size\n       fast block write: &, receives data, then % or !...
 *   Z\n                       finish write session/reset
 *   Roffset,size\n            read raw bytes
 *   Koffset,size\n            CRC32, 8 ASCII hex chars
 */

#include "FlashTypes.h"
#include "MX29L3211Device.h"
#include "MX26L6420SocketDevice.h"
#include "MX29LV160CTSocketDevice.h"
#include <stdlib.h>
#include <string.h>

#define SERIAL_BAUD 500000
#define CMD_BUFFER_SIZE 96
#define MAX_WRITE_BLOCK 4096
#define RX_TIMEOUT_MS 10000UL

#define ACK_READY '&'
#define ACK_DONE  '%'
#define ACK_ERROR '!'

MX29L3211Device mx29l3211;
MX26L6420SocketDevice mx26l6420;
MX29LV160CTSocketDevice mx29lv160ct;

static byte writeBuffer[MAX_WRITE_BLOCK];
static char commandBuffer[CMD_BUFFER_SIZE];
static uint8_t commandLen = 0;

static ChipProfileId activeProfile = PROFILE_NONE;
static bool profileLocked = false;
static bool idOkLatch = false;
static bool eraseOkLatch = false;
static bool writeSessionActive = false;

static bool parseLongs(char *s, long *values, uint8_t count) {
  for (uint8_t i = 0; i < count; i++) {
    char *endPtr = NULL;
    values[i] = strtol(s, &endPtr, 10);
    if (endPtr == s) return false;
    if (i < count - 1) {
      if (*endPtr != ',') return false;
      s = endPtr + 1;
    } else if (*endPtr != '\0') {
      return false;
    }
  }
  return true;
}

static void printHex16(uint16_t value) {
  char buf[5];
  sprintf(buf, "%04X", value);
  Serial.print(buf);
}

static void printHex32(uint32_t value) {
  char buf[9];
  sprintf(buf, "%08lX", (unsigned long)value);
  Serial.print(buf);
}

static void sendError(char code) {
  Serial.write(ACK_ERROR);
  Serial.write(code);
  Serial.write('\n');
}

static void sendDriverError(char fallbackCode, FlashErrorInfo error) {
  char code = error.code == '0' ? fallbackCode : error.code;
  Serial.write(ACK_ERROR);
  Serial.print(code);
  Serial.print(F(",OFF=0x"));
  printHex32(error.byteOffset);
  Serial.print(F(",EXP=0x"));
  printHex16(error.expected);
  Serial.print(F(",ACT=0x"));
  printHex16(error.actual);
  Serial.write('\n');
}

static bool receiveExact(byte *buffer, uint32_t size) {
  uint32_t idx = 0;
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

static void universalSafePins() {
  // Deselect chip first and hold all possible control pins inactive/high.
  DDRH |= (1 << 1) | (1 << 3) | (1 << 4) | (1 << 6);
  PORTH |= (1 << 1) | (1 << 3) | (1 << 4) | (1 << 6);

  DDRL = 0x1F;
  PORTL |= (1 << 4); // Safe if pin 44 is WE; harmless while CE is high if it is A20.

  DDRF = 0xFF;
  DDRK = 0xFF;
  PORTF = 0x00;
  PORTK = 0x00;
  PORTL &= 0xF0;
  PORTL |= (1 << 4);

  DDRC = 0x00;
  DDRA = 0x00;
  PORTC = 0x00;
  PORTA = 0x00;
}

static uint32_t profileSize(ChipProfileId id) {
  switch (id) {
    case PROFILE_MX29L3211: return MX29L3211Device::SIZE_BYTES;
    case PROFILE_MX26L6420_SOCKET: return MX26L6420SocketDevice::SIZE_BYTES;
    case PROFILE_MX29LV160CT_SOCKET: return MX29LV160CTSocketDevice::SIZE_BYTES;
    default: return 0;
  }
}

static uint16_t profileManufacturer(ChipProfileId id) {
  switch (id) {
    case PROFILE_MX29L3211: return MX29L3211Device::MANUFACTURER_ID;
    case PROFILE_MX26L6420_SOCKET: return MX26L6420SocketDevice::MANUFACTURER_ID;
    case PROFILE_MX29LV160CT_SOCKET: return MX29LV160CTSocketDevice::MANUFACTURER_ID;
    default: return 0;
  }
}

static uint16_t profileDevice(ChipProfileId id) {
  switch (id) {
    case PROFILE_MX29L3211: return MX29L3211Device::DEVICE_ID;
    case PROFILE_MX26L6420_SOCKET: return MX26L6420SocketDevice::DEVICE_ID;
    case PROFILE_MX29LV160CT_SOCKET: return MX29LV160CTSocketDevice::DEVICE_ID;
    default: return 0;
  }
}

static uint16_t profileDefaultPage(ChipProfileId id) {
  return id == PROFILE_MX29L3211 ? 256 : 2;
}

static bool profileFastValidated(ChipProfileId id) {
  return id == PROFILE_MX29L3211 || id == PROFILE_MX26L6420_SOCKET;
}

static bool profileHardwareValidated(ChipProfileId id) {
  return id == PROFILE_MX29L3211 || id == PROFILE_MX26L6420_SOCKET;
}

static void printProfileName(ChipProfileId id) {
  switch (id) {
    case PROFILE_MX29L3211: Serial.print(F("MX29L3211")); break;
    case PROFILE_MX26L6420_SOCKET: Serial.print(F("MX26L6420_SOCKET")); break;
    case PROFILE_MX29LV160CT_SOCKET: Serial.print(F("MX29LV160CTMC_SOCKET")); break;
    default: Serial.print(F("NONE")); break;
  }
}

static void printProfileInfo(ChipProfileId id) {
  Serial.print(F("PROFILE:ID=")); Serial.print((uint8_t)id);
  Serial.print(F(",NAME=")); printProfileName(id);
  Serial.print(F(",SIZE=")); Serial.print(profileSize(id));
  Serial.print(F(",MFR=")); printHex16(profileManufacturer(id));
  Serial.print(F(",DEV=")); printHex16(profileDevice(id));
  Serial.print(F(",PAGE=")); Serial.print(profileDefaultPage(id));
  Serial.print(F(",READBLOCK=16384,WRITEBLOCK=4096"));
  Serial.print(F(",FAST=")); Serial.print(profileFastValidated(id) ? 1 : 0);
  Serial.print(F(",VALIDATED=")); Serial.print(profileHardwareValidated(id) ? 1 : 0);
  Serial.print(F(",LOCKED=")); Serial.print(profileLocked ? 1 : 0);
  Serial.print(F(",IDOK=")); Serial.print(idOkLatch ? 1 : 0);
  Serial.print(F(",ERASEOK=")); Serial.println(eraseOkLatch ? 1 : 0);
}

static void deactivateActiveProfile() {
  switch (activeProfile) {
    case PROFILE_MX29L3211: mx29l3211.deactivate(); break;
    case PROFILE_MX26L6420_SOCKET: mx26l6420.deactivate(); break;
    case PROFILE_MX29LV160CT_SOCKET: mx29lv160ct.deactivate(); break;
    default: break;
  }
}

static void activateProfile(ChipProfileId id) {
  deactivateActiveProfile();
  universalSafePins();
  activeProfile = id;
  idOkLatch = false;
  eraseOkLatch = false;
  writeSessionActive = false;
  profileLocked = false;

  switch (id) {
    case PROFILE_MX29L3211: mx29l3211.activate(); break;
    case PROFILE_MX26L6420_SOCKET: mx26l6420.activate(); break;
    case PROFILE_MX29LV160CT_SOCKET: mx29lv160ct.activate(); break;
    default: break;
  }
}

static bool profileKeyValid(ChipProfileId id, const char *key) {
  if (!key) return false;
  switch (id) {
    case PROFILE_MX29L3211: return strcmp(key, "3211") == 0;
    case PROFILE_MX26L6420_SOCKET: return strcmp(key, "6420") == 0;
    case PROFILE_MX29LV160CT_SOCKET: return strcmp(key, "160C") == 0;
    default: return false;
  }
}

static bool driverReadId(uint16_t &manufacturer, uint16_t &device) {
  switch (activeProfile) {
    case PROFILE_MX29L3211: return mx29l3211.readId(manufacturer, device);
    case PROFILE_MX26L6420_SOCKET: return mx26l6420.readId(manufacturer, device);
    case PROFILE_MX29LV160CT_SOCKET: return mx29lv160ct.readId(manufacturer, device);
    default: return false;
  }
}

static void driverReset() {
  switch (activeProfile) {
    case PROFILE_MX29L3211: mx29l3211.reset(); break;
    case PROFILE_MX26L6420_SOCKET: mx26l6420.reset(); break;
    case PROFILE_MX29LV160CT_SOCKET: mx29lv160ct.reset(); break;
    default: break;
  }
}

static bool driverErase() {
  switch (activeProfile) {
    case PROFILE_MX29L3211: return mx29l3211.erase();
    case PROFILE_MX26L6420_SOCKET: return mx26l6420.erase();
    case PROFILE_MX29LV160CT_SOCKET: return mx29lv160ct.erase();
    default: return false;
  }
}

static void driverReadBytes(uint32_t offset, uint32_t size) {
  switch (activeProfile) {
    case PROFILE_MX29L3211: mx29l3211.readBytes(offset, size); break;
    case PROFILE_MX26L6420_SOCKET: mx26l6420.readBytes(offset, size); break;
    case PROFILE_MX29LV160CT_SOCKET: mx29lv160ct.readBytes(offset, size); break;
    default: break;
  }
}

static bool driverPageSizeValid(uint16_t pageSize) {
  switch (activeProfile) {
    case PROFILE_MX29L3211: return mx29l3211.pageSizeValid(pageSize);
    case PROFILE_MX26L6420_SOCKET: return mx26l6420.pageSizeValid(pageSize);
    case PROFILE_MX29LV160CT_SOCKET: return mx29lv160ct.pageSizeValid(pageSize);
    default: return false;
  }
}

static bool driverWriteBlock(uint32_t offset, uint16_t pageSize,
                             uint32_t size, const byte *data) {
  switch (activeProfile) {
    case PROFILE_MX29L3211: return mx29l3211.writeBlock(offset, pageSize, size, data);
    case PROFILE_MX26L6420_SOCKET: return mx26l6420.writeBlock(offset, pageSize, size, data);
    case PROFILE_MX29LV160CT_SOCKET: return mx29lv160ct.writeBlock(offset, pageSize, size, data);
    default: return false;
  }
}

static FlashErrorInfo driverLastError() {
  switch (activeProfile) {
    case PROFILE_MX29L3211: return mx29l3211.lastError();
    case PROFILE_MX26L6420_SOCKET: return mx26l6420.lastError();
    case PROFILE_MX29LV160CT_SOCKET: return mx29lv160ct.lastError();
    default: {
      FlashErrorInfo e; clearFlashError(e); e.code = 'N'; return e;
    }
  }
}

static uint32_t driverCrc32(uint32_t offset, uint32_t size) {
  switch (activeProfile) {
    case PROFILE_MX29L3211: return mx29l3211.crc32(offset, size);
    case PROFILE_MX26L6420_SOCKET: return mx26l6420.crc32(offset, size);
    case PROFILE_MX29LV160CT_SOCKET: return mx29lv160ct.crc32(offset, size);
    default: return 0;
  }
}

static bool destructiveReady() {
  return activeProfile != PROFILE_NONE && profileLocked && idOkLatch;
}

static void commandVersion() {
  Serial.print(F("MBW_MULTICHIP v0.7A BAUD=")); Serial.print(SERIAL_BAUD);
  Serial.println(F(" PROFILES=1:MX29L3211;2:MX26L6420_SOCKET;3:MX29LV160CTMC_SOCKET"));
}

static void commandSelectProfile(char *param) {
  if (profileLocked || writeSessionActive) {
    sendError('L');
    return;
  }

  char *comma = strchr(param, ',');
  if (!comma) {
    sendError('C');
    return;
  }
  *comma = '\0';
  long idValue = strtol(param, NULL, 10);
  ChipProfileId id = (ChipProfileId)idValue;
  const char *key = comma + 1;
  if (id < PROFILE_MX29L3211 || id > PROFILE_MX29LV160CT_SOCKET || !profileKeyValid(id, key)) {
    sendError('K');
    return;
  }

  activateProfile(id);
  printProfileInfo(id);
}

static void commandQueryProfile() {
  printProfileInfo(activeProfile);
}

static void commandCheckId() {
  if (activeProfile == PROFILE_NONE) {
    sendError('N');
    return;
  }

  uint16_t manufacturer = 0;
  uint16_t device = 0;
  bool ok = driverReadId(manufacturer, device);
  idOkLatch = ok && manufacturer == profileManufacturer(activeProfile) &&
              device == profileDevice(activeProfile);
  profileLocked = idOkLatch;
  eraseOkLatch = false;

  Serial.print(F("ID:PROFILE=")); Serial.print((uint8_t)activeProfile);
  Serial.print(F(",MFR=")); printHex16(manufacturer);
  Serial.print(F(",DEV=")); printHex16(device);
  Serial.print(F(",EXPECTED=")); printHex16(profileManufacturer(activeProfile));
  Serial.write('/'); printHex16(profileDevice(activeProfile));
  Serial.print(F(",OK=")); Serial.println(idOkLatch ? 1 : 0);
}

static void commandErase() {
  if (!destructiveReady() || writeSessionActive) {
    sendError('I');
    return;
  }

  Serial.write(ACK_READY);
  Serial.flush();
  bool ok = driverErase();
  if (ok) {
    driverReset();
    eraseOkLatch = true;
    Serial.write(ACK_DONE);
  } else {
    eraseOkLatch = false;
    sendDriverError('E', driverLastError());
  }
}

static void commandBeginWrite() {
  if (!destructiveReady() || !eraseOkLatch || writeSessionActive) {
    sendError('I');
    return;
  }
  driverReset();
  writeSessionActive = true;
  Serial.write(ACK_DONE);
}

static void commandFinishWrite() {
  if (activeProfile == PROFILE_NONE) {
    sendError('N');
    return;
  }
  driverReset();
  writeSessionActive = false;
  eraseOkLatch = false;
  Serial.write(ACK_DONE);
}

static void commandWriteBlock(char *param, bool fastMode) {
  if (!destructiveReady() || !eraseOkLatch || !writeSessionActive) {
    sendError('I');
    return;
  }
  if (fastMode && !profileFastValidated(activeProfile)) {
    sendError('F');
    return;
  }

  long values[3];
  if (!parseLongs(param, values, 3)) {
    sendError('C');
    return;
  }
  long offset = values[0];
  long pageSize = values[1];
  long blockSize = values[2];

  if (offset < 0 || pageSize <= 0 || blockSize <= 0 || blockSize > MAX_WRITE_BLOCK) {
    sendError('B');
    return;
  }
  if ((offset & 1L) || (blockSize & 1L)) {
    sendError('A');
    return;
  }
  if ((uint32_t)offset + (uint32_t)blockSize > profileSize(activeProfile)) {
    sendError('O');
    return;
  }
  if (!driverPageSizeValid((uint16_t)pageSize)) {
    sendError('G');
    return;
  }

  Serial.write(ACK_READY);
  if (!receiveExact(writeBuffer, (uint32_t)blockSize)) {
    sendError('R');
    return;
  }

  bool ok = driverWriteBlock((uint32_t)offset, (uint16_t)pageSize,
                             (uint32_t)blockSize, writeBuffer);
  if (!ok) {
    sendDriverError('P', driverLastError());
    return;
  }

  if (!fastMode) driverReset();
  Serial.write(ACK_DONE);
}

static void commandRead(char *param) {
  if (!destructiveReady() || writeSessionActive) {
    sendError('I');
    return;
  }
  long values[2];
  if (!parseLongs(param, values, 2)) {
    sendError('C');
    return;
  }
  long offset = values[0];
  long size = values[1];
  if (offset < 0 || size <= 0 || (offset & 1L) || (size & 1L) ||
      (uint32_t)offset + (uint32_t)size > profileSize(activeProfile)) {
    sendError('B');
    return;
  }
  Serial.write(ACK_READY);
  driverReadBytes((uint32_t)offset, (uint32_t)size);
}

static void commandCrc(char *param) {
  if (!destructiveReady() || writeSessionActive) {
    sendError('I');
    return;
  }
  long values[2];
  if (!parseLongs(param, values, 2)) {
    sendError('C');
    return;
  }
  long offset = values[0];
  long size = values[1];
  if (offset < 0 || size <= 0 || (uint32_t)offset + (uint32_t)size > profileSize(activeProfile)) {
    sendError('B');
    return;
  }
  if (activeProfile != PROFILE_MX29L3211 && ((offset & 1L) || (size & 1L))) {
    sendError('A');
    return;
  }
  printHex32(driverCrc32((uint32_t)offset, (uint32_t)size));
}

static void processCommand(char *cmd) {
  if (!cmd || cmd[0] == '\0') return;

  switch (cmd[0]) {
    case 'V': commandVersion(); break;
    case 'P': commandSelectProfile(cmd + 1); break;
    case 'Q': commandQueryProfile(); break;
    case 'C': commandCheckId(); break;
    case 'E': commandErase(); break;
    case 'S': commandBeginWrite(); break;
    case 'W': commandWriteBlock(cmd + 1, false); break;
    case 'X': commandWriteBlock(cmd + 1, true); break;
    case 'Z': commandFinishWrite(); break;
    case 'R': commandRead(cmd + 1); break;
    case 'K': commandCrc(cmd + 1); break;
    default: sendError('C'); break;
  }
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  universalSafePins();
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
