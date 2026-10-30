#ifndef MX26L6420_SOCKET_DEVICE_H
#define MX26L6420_SOCKET_DEVICE_H

#include <Arduino.h>
#include "FlashTypes.h"

class MX26L6420SocketDevice {
public:
  static const uint32_t SIZE_BYTES = 8388608UL;
  static const uint16_t MANUFACTURER_ID = 0x00C2;
  static const uint16_t DEVICE_ID = 0x22FC;

  MX26L6420SocketDevice();
  void activate();
  void deactivate();
  void reset();
  bool readId(uint16_t &manufacturer, uint16_t &device);
  void readBytes(uint32_t byteOffset, uint32_t byteCount);
  bool erase();
  bool writeBlock(uint32_t byteOffset, uint16_t pageSizeBytes, uint32_t blockSizeBytes, const byte *data);
  uint32_t crc32(uint32_t byteOffset, uint32_t byteCount);
  bool pageSizeValid(uint16_t pageSizeBytes) const { return pageSizeBytes == 2; }
  FlashErrorInfo lastError() const { return error; }

private:
  static const uint8_t OE_BIT = 1;   // PH1 / D16
  static const uint8_t WE_BIT = 3;   // PH3 / D6  -> MX26 pin 33 WE
  static const uint8_t A21_BIT = 4;  // PH4 / D7  -> MX26 pin 1 A21
  static const uint8_t CE_BIT = 6;   // PH6 / D9
  static const uint32_t UNLOCK1 = 0x0555UL;
  static const uint32_t UNLOCK2 = 0x02AAUL;

  FlashErrorInfo error;

  inline void dataOut() { DDRC = 0xFF; DDRA = 0xFF; }
  inline void dataIn()  { DDRC = 0x00; DDRA = 0x00; }

  inline void setAddress(uint32_t wordAddress) {
    PORTF = wordAddress & 0xFF;
    PORTK = (wordAddress >> 8) & 0xFF;
    PORTL = (PORTL & 0xE0) | ((wordAddress >> 16) & 0x1F); // A16-A20
    if (wordAddress & 0x200000UL) PORTH |= (1 << A21_BIT);
    else PORTH &= ~(1 << A21_BIT);
  }

  void writeWord(uint32_t wordAddress, uint16_t data);
  uint16_t readWord(uint32_t wordAddress);
  bool waitToggleReady(uint32_t wordAddress, uint16_t expected, uint32_t timeoutMs);
};

#endif
