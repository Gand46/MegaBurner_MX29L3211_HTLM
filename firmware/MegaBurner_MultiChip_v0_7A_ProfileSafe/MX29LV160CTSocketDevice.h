#ifndef MX29LV160CT_SOCKET_DEVICE_H
#define MX29LV160CT_SOCKET_DEVICE_H

#include <Arduino.h>
#include "FlashTypes.h"

class MX29LV160CTSocketDevice {
public:
  static const uint32_t SIZE_BYTES = 2097152UL;      // 16 Mbit = 2 MiB
  static const uint16_t MANUFACTURER_ID = 0x00C2;
  static const uint16_t DEVICE_ID = 0x22C4;          // Top boot variant

  MX29LV160CTSocketDevice();
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
  // MX29L3211-compatible physical socket:
  // PH4 / D7  -> SOP44 pin 1  -> RESET# on MX29LV160CT
  // PH3 / D6  -> SOP44 pin 33 -> BYTE#  on MX29LV160CT
  // PL4 / D45 -> SOP44 pin 44 -> WE#    on MX29LV160CT
  static const uint8_t OE_BIT = 1;      // PH1 / D16
  static const uint8_t BYTE_BIT = 3;    // PH3 / D6
  static const uint8_t RESET_BIT = 4;   // PH4 / D7
  static const uint8_t CE_BIT = 6;      // PH6 / D9
  static const uint8_t WE_PL_BIT = 4;   // PL4 / D45
  static const uint32_t UNLOCK1 = 0x0555UL;
  static const uint32_t UNLOCK2 = 0x02AAUL;
  static const uint8_t MAX_REPAIR_TRIES = 2;

  FlashErrorInfo error;

  inline void dataOut() { DDRC = 0xFF; DDRA = 0xFF; }
  inline void dataIn()  {
    DDRC = 0x00;
    DDRA = 0x00;
    PORTC = 0x00;
    PORTA = 0x00;
  }

  inline void setAddress(uint32_t wordAddress) {
    PORTF = wordAddress & 0xFF;
    PORTK = (wordAddress >> 8) & 0xFF;
    // A16-A19 use PL0-PL3. Preserve PL4 because it is WE# for this chip.
    PORTL = (PORTL & 0xF0) | ((wordAddress >> 16) & 0x0F);
  }

  void writeWord(uint32_t wordAddress, uint16_t data);
  uint16_t readWord(uint32_t wordAddress);
  bool waitToggleReady(uint32_t wordAddress, uint32_t timeoutMs);
  bool programWord(uint32_t wordAddress, uint16_t data);
  bool verifyAndRepairBlock(uint32_t byteOffset, uint32_t blockSizeBytes, const byte *data);
  bool blankSampleCheck();
};

#endif
