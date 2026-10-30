#ifndef FLASH_TYPES_H
#define FLASH_TYPES_H

#include <Arduino.h>

enum ChipProfileId : uint8_t {
  PROFILE_NONE = 0,
  PROFILE_MX29L3211 = 1,
  PROFILE_MX26L6420_SOCKET = 2,
  PROFILE_MX29LV160CT_SOCKET = 3
};

struct FlashErrorInfo {
  char code;                // '0' = no error
  uint32_t byteOffset;
  uint16_t expected;
  uint16_t actual;
};

inline void clearFlashError(FlashErrorInfo &e) {
  e.code = '0';
  e.byteOffset = 0;
  e.expected = 0;
  e.actual = 0;
}

inline uint32_t flashCrc32Update(uint32_t crc, byte data) {
  crc ^= data;
  for (byte i = 0; i < 8; i++) {
    crc = (crc & 1U) ? ((crc >> 1) ^ 0xEDB88320UL) : (crc >> 1);
  }
  return crc;
}

#endif
