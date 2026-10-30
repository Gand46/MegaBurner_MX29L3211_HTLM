#ifndef MX29L3211_DEVICE_H
#define MX29L3211_DEVICE_H

#include <Arduino.h>
#include "FlashTypes.h"

class MX29L3211Device {
public:
  static const uint32_t SIZE_BYTES = 4194304UL;
  static const uint16_t MANUFACTURER_ID = 0x00C2;
  static const uint16_t DEVICE_ID = 0x00F9;

  MX29L3211Device();
  void activate();
  void deactivate();
  void reset();
  bool readId(uint16_t &manufacturer, uint16_t &device);
  void readBytes(uint32_t byteOffset, uint32_t byteCount);
  bool erase();
  bool writeBlock(uint32_t byteOffset, uint16_t pageSizeBytes, uint32_t blockSizeBytes, const byte *data);
  uint32_t crc32(uint32_t byteOffset, uint32_t byteCount);
  bool pageSizeValid(uint16_t pageSizeBytes) const;
  FlashErrorInfo lastError() const { return error; }

private:
  FlashErrorInfo error;

  inline void dataOut() { DDRC = 0xFF; DDRA = 0xFF; }
  inline void dataIn()  { DDRC = 0x00; DDRA = 0x00; }

  void writeWord(uint32_t wordAddress, uint16_t data);
  uint16_t readWord(uint32_t wordAddress);
  byte readByte(uint32_t byteAddress);
  uint16_t readStatusReg();
  void busyCheck();
};

#endif
