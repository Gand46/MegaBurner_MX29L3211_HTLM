#include "MX29LV160CTSocketDevice.h"

MX29LV160CTSocketDevice::MX29LV160CTSocketDevice() {
  clearFlashError(error);
}

void MX29LV160CTSocketDevice::activate() {
  clearFlashError(error);

  DDRF = 0xFF;          // A0-A7
  DDRK = 0xFF;          // A8-A15
  DDRL = 0x1F;         // PL0-PL3 A16-A19, PL4 WE#
  DDRH |= (1 << OE_BIT) | (1 << BYTE_BIT) | (1 << RESET_BIT) | (1 << CE_BIT);

  dataIn();

  PORTH |= (1 << OE_BIT) | (1 << BYTE_BIT) | (1 << RESET_BIT); // OE/BYTE/RESET high
  PORTH &= ~(1 << CE_BIT);                                      // CE low
  PORTL |= (1 << WE_PL_BIT);                                    // WE high
  setAddress(0);
  delay(10);
}

void MX29LV160CTSocketDevice::deactivate() {
  PORTH |= (1 << OE_BIT) | (1 << BYTE_BIT) | (1 << RESET_BIT) | (1 << CE_BIT);
  PORTL |= (1 << WE_PL_BIT);
  dataIn();
}

void MX29LV160CTSocketDevice::writeWord(uint32_t wordAddress, uint16_t data) {
  PORTH |= (1 << OE_BIT);       // OE high during writes
  PORTH |= (1 << RESET_BIT);    // RESET inactive/high
  PORTH |= (1 << BYTE_BIT);     // x16 mode
  dataOut();
  setAddress(wordAddress);
  PORTC = (byte)(data & 0xFF);
  PORTA = (byte)((data >> 8) & 0xFF);

  __asm__("nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t");
  PORTL &= ~(1 << WE_PL_BIT);
  __asm__("nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t");
  PORTL |= (1 << WE_PL_BIT);
  __asm__("nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t");
}

uint16_t MX29LV160CTSocketDevice::readWord(uint32_t wordAddress) {
  dataIn();
  PORTH |= (1 << RESET_BIT) | (1 << BYTE_BIT);
  PORTL |= (1 << WE_PL_BIT);
  setAddress(wordAddress);
  __asm__("nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t");

  PORTH &= ~(1 << OE_BIT);
  __asm__("nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t");
  uint16_t value = ((PINA & 0xFF) << 8) | (PINC & 0xFF);
  PORTH |= (1 << OE_BIT);
  __asm__("nop\n\t" "nop\n\t" "nop\n\t");
  return value;
}

void MX29LV160CTSocketDevice::reset() {
  PORTH |= (1 << RESET_BIT) | (1 << BYTE_BIT);
  writeWord(0x0000UL, 0x00F0);
  dataIn();
  delay(10);
}

bool MX29LV160CTSocketDevice::readId(uint16_t &manufacturer, uint16_t &device) {
  writeWord(UNLOCK1, 0x00AA);
  writeWord(UNLOCK2, 0x0055);
  writeWord(UNLOCK1, 0x0090);
  dataIn();
  manufacturer = readWord(0x0000UL);
  device = readWord(0x0001UL);
  reset();
  return manufacturer == MANUFACTURER_ID && device == DEVICE_ID;
}

void MX29LV160CTSocketDevice::readBytes(uint32_t byteOffset, uint32_t byteCount) {
  uint32_t startWord = byteOffset / 2;
  uint32_t words = byteCount / 2;
  dataIn();
  for (uint32_t i = 0; i < words; i++) {
    uint16_t w = readWord(startWord + i);
    Serial.write((byte)(w & 0xFF));
    Serial.write((byte)((w >> 8) & 0xFF));
  }
}

bool MX29LV160CTSocketDevice::waitToggleReady(uint32_t wordAddress, uint32_t timeoutMs) {
  dataIn();
  unsigned long start = millis();
  while (millis() - start < timeoutMs) {
    uint16_t a = readWord(wordAddress);
    uint16_t b = readWord(wordAddress);
    if (((a ^ b) & 0x0040) == 0) return true;

    if (b & 0x0020) {
      uint16_t c = readWord(wordAddress);
      uint16_t d = readWord(wordAddress);
      if (((c ^ d) & 0x0040) == 0) return true;
      return false;
    }
  }
  return false;
}

bool MX29LV160CTSocketDevice::programWord(uint32_t wordAddress, uint16_t data) {
  writeWord(UNLOCK1, 0x00AA);
  writeWord(UNLOCK2, 0x0055);
  writeWord(UNLOCK1, 0x00A0);
  writeWord(wordAddress, data);
  return waitToggleReady(wordAddress, 1000UL);
}

bool MX29LV160CTSocketDevice::blankSampleCheck() {
  static const uint32_t sampleWords[] = {
    0x000000UL, 0x000001UL, 0x000555UL, 0x002AAUL,
    0x010000UL, 0x07FFFFUL, 0x080000UL, 0x0FFFFFUL
  };
  for (uint8_t i = 0; i < sizeof(sampleWords) / sizeof(sampleWords[0]); i++) {
    if (readWord(sampleWords[i]) != 0xFFFF) return false;
  }
  return true;
}

bool MX29LV160CTSocketDevice::erase() {
  clearFlashError(error);
  writeWord(UNLOCK1, 0x00AA);
  writeWord(UNLOCK2, 0x0055);
  writeWord(UNLOCK1, 0x0080);
  writeWord(UNLOCK1, 0x00AA);
  writeWord(UNLOCK2, 0x0055);
  writeWord(UNLOCK1, 0x0010);

  bool ok = waitToggleReady(0x0000UL, 300000UL);
  reset();
  if (!ok || !blankSampleCheck()) {
    error.code = 'E';
    return false;
  }
  return true;
}

bool MX29LV160CTSocketDevice::verifyAndRepairBlock(uint32_t byteOffset,
                                                    uint32_t blockSizeBytes,
                                                    const byte *data) {
  reset();
  for (uint32_t bi = 0; bi < blockSizeBytes; bi += 2) {
    uint32_t wordAddress = (byteOffset + bi) / 2;
    uint16_t expected = ((data[bi + 1] & 0xFF) << 8) | (data[bi] & 0xFF);
    uint16_t actual = readWord(wordAddress);
    if (actual == expected) continue;

    if ((actual & expected) != expected) {
      error.code = 'Z';
      error.byteOffset = byteOffset + bi;
      error.expected = expected;
      error.actual = actual;
      return false;
    }

    bool repaired = false;
    for (uint8_t attempt = 0; attempt < MAX_REPAIR_TRIES; attempt++) {
      if (!programWord(wordAddress, expected)) break;
      reset();
      actual = readWord(wordAddress);
      if (actual == expected) {
        repaired = true;
        break;
      }
    }
    if (!repaired) {
      error.code = 'V';
      error.byteOffset = byteOffset + bi;
      error.expected = expected;
      error.actual = actual;
      return false;
    }
  }
  return true;
}

bool MX29LV160CTSocketDevice::writeBlock(uint32_t byteOffset,
                                         uint16_t pageSizeBytes,
                                         uint32_t blockSizeBytes,
                                         const byte *data) {
  clearFlashError(error);
  if (!pageSizeValid(pageSizeBytes) || (byteOffset & 1U) || (blockSizeBytes & 1U) ||
      blockSizeBytes == 0 || byteOffset + blockSizeBytes > SIZE_BYTES) {
    error.code = 'B';
    error.byteOffset = byteOffset;
    return false;
  }

  for (uint32_t bi = 0; bi < blockSizeBytes; bi += 2) {
    uint32_t wordAddress = (byteOffset + bi) / 2;
    uint16_t wordData = ((data[bi + 1] & 0xFF) << 8) | (data[bi] & 0xFF);
    if (!programWord(wordAddress, wordData)) {
      error.code = 'P';
      error.byteOffset = byteOffset + bi;
      error.expected = wordData;
      reset();
      error.actual = readWord(wordAddress);
      return false;
    }
  }
  return verifyAndRepairBlock(byteOffset, blockSizeBytes, data);
}

uint32_t MX29LV160CTSocketDevice::crc32(uint32_t byteOffset, uint32_t byteCount) {
  uint32_t crc = 0xFFFFFFFFUL;
  uint32_t startWord = byteOffset / 2;
  uint32_t words = byteCount / 2;
  dataIn();
  for (uint32_t i = 0; i < words; i++) {
    uint16_t w = readWord(startWord + i);
    crc = flashCrc32Update(crc, (byte)(w & 0xFF));
    crc = flashCrc32Update(crc, (byte)((w >> 8) & 0xFF));
  }
  return ~crc;
}
