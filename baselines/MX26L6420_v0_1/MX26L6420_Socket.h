#ifndef MX26L6420_SOCKET_H
#define MX26L6420_SOCKET_H

#include <Arduino.h>

class MX26L6420Socket {
public:
  static const uint32_t SIZE_BYTES = 8388608UL;      // 64 Mbit = 8 MiB
  static const uint32_t SIZE_WORDS = 4194304UL;      // 4M x 16
  static const uint16_t MANUFACTURER_ID = 0x00C2;
  static const uint16_t DEVICE_ID = 0x22FC;

  MX26L6420Socket();
  void init();
  void reset();
  void readId(uint16_t *manufacturer, uint16_t *device);
  void read16(uint32_t blockId, uint32_t blockSizeBytes);
  bool erase();
  bool write16WordBlock(uint32_t byteOffset, uint32_t blockSizeBytes, const byte data[]);
  uint32_t crc32_16(uint32_t byteOffset, uint32_t byteCount);

private:
  // MX29L3211-compatible socket pin swap for MX26L6420 SOP44:
  // PH4 / Arduino D7  = SOP44 pin 1  = A21 on MX26L6420
  // PH3 / Arduino D6  = SOP44 pin 33 = WE  on MX26L6420
  static const uint8_t OE_BIT  = 1; // PH1 / D16 / SOP44 pin 14
  static const uint8_t WE_BIT  = 3; // PH3 / D6  / SOP44 pin 33 on MX26L6420
  static const uint8_t A21_BIT = 4; // PH4 / D7  / SOP44 pin 1  on MX26L6420
  static const uint8_t CE_BIT  = 6; // PH6 / D9  / SOP44 pin 12

  static const uint32_t UNLOCK1 = 0x0555UL;
  static const uint32_t UNLOCK2 = 0x02AAUL;

  inline void dataOut() { DDRC = 0xFF; DDRA = 0xFF; }
  inline void dataIn()  { DDRC = 0x00; DDRA = 0x00; }

  inline void setAddress(uint32_t wordAddress) {
    // Low and middle address bytes remain identical to MX29L3211 socket wiring.
    PORTF = wordAddress & 0xFF;                 // A0-A7
    PORTK = (wordAddress >> 8) & 0xFF;          // A8-A15

    // A16-A20 remain on PORTL bits 0-4. Preserve unused PL5-PL7.
    PORTL = (PORTL & 0xE0) | ((wordAddress >> 16) & 0x1F);

    // A21 is NOT on PORTL in this socket-compatible mode.
    // It is on the old MX29L3211 WE line: PH4 / D7 / SOP44 pin 1.
    if (wordAddress & 0x200000UL) PORTH |=  (1 << A21_BIT);
    else                         PORTH &= ~(1 << A21_BIT);
  }

  void writeWord(uint32_t wordAddress, uint16_t data);
  uint16_t readWord(uint32_t wordAddress);
  bool waitToggleReady(uint32_t wordAddress, uint16_t expected, uint32_t timeoutMs);
  static uint32_t crc32_update(uint32_t crc, byte data);
};

#endif
