#include "MX26L6420SocketDevice.h"

MX26L6420SocketDevice::MX26L6420SocketDevice() {
  clearFlashError(error);
}

void MX26L6420SocketDevice::activate() {
  clearFlashError(error);
  DDRF = 0xFF;
  DDRK = 0xFF;
  DDRL = 0x1F;
  DDRH |= (1 << OE_BIT) | (1 << WE_BIT) | (1 << A21_BIT) | (1 << CE_BIT);

  dataIn();
  PORTC = 0x00;
  PORTA = 0x00;

  PORTH |= (1 << OE_BIT) | (1 << WE_BIT);
  PORTH &= ~(1 << A21_BIT);
  PORTH &= ~(1 << CE_BIT);
  setAddress(0);
  delay(10);
}

void MX26L6420SocketDevice::deactivate() {
  PORTH |= (1 << OE_BIT) | (1 << WE_BIT) | (1 << A21_BIT) | (1 << CE_BIT);
  dataIn();
  PORTC = 0x00;
  PORTA = 0x00;
}

void MX26L6420SocketDevice::writeWord(uint32_t wordAddress, uint16_t data) {
  dataOut();
  setAddress(wordAddress);
  PORTC = (byte)(data & 0xFF);
  PORTA = (byte)((data >> 8) & 0xFF);
  __asm__("nop\n\t" "nop\n\t");
  PORTH &= ~(1 << WE_BIT);
  __asm__("nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t");
  PORTH |= (1 << WE_BIT);
  __asm__("nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t");
}

uint16_t MX26L6420SocketDevice::readWord(uint32_t wordAddress) {
  setAddress(wordAddress);
  __asm__("nop\n\t" "nop\n\t");
  PORTH &= ~(1 << OE_BIT);
  __asm__("nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t");
  uint16_t value = ((PINA & 0xFF) << 8) | (PINC & 0xFF);
  PORTH |= (1 << OE_BIT);
  __asm__("nop\n\t" "nop\n\t" "nop\n\t");
  return value;
}

void MX26L6420SocketDevice::reset() {
  writeWord(0x0000UL, 0x00F0);
  dataIn();
  delay(10);
}

bool MX26L6420SocketDevice::readId(uint16_t &manufacturer, uint16_t &device) {
  writeWord(UNLOCK1, 0x00AA);
  writeWord(UNLOCK2, 0x0055);
  writeWord(UNLOCK1, 0x0090);
  dataIn();
  manufacturer = readWord(0x0000UL);
  device = readWord(0x0001UL);
  reset();
  return ((manufacturer & 0x00FF) == (MANUFACTURER_ID & 0x00FF)) &&
         (device == DEVICE_ID || (device & 0x00FF) == (DEVICE_ID & 0x00FF));
}

void MX26L6420SocketDevice::readBytes(uint32_t byteOffset, uint32_t byteCount) {
  uint32_t startWord = byteOffset / 2;
  uint32_t words = byteCount / 2;
  dataIn();
  for (uint32_t i = 0; i < words; i++) {
    uint16_t w = readWord(startWord + i);
    Serial.write((byte)(w & 0xFF));
    Serial.write((byte)((w >> 8) & 0xFF));
  }
}

bool MX26L6420SocketDevice::waitToggleReady(uint32_t wordAddress, uint16_t expected,
                                             uint32_t timeoutMs) {
  dataIn();
  unsigned long start = millis();
  while (millis() - start < timeoutMs) {
    uint16_t a = readWord(wordAddress);
    uint16_t b = readWord(wordAddress);

    if (((a ^ b) & 0x0040) == 0) {
      uint16_t finalValue = readWord(wordAddress);
      return (finalValue == expected) || ((finalValue & 0x0080) == (expected & 0x0080));
    }

    if (b & 0x0020) {
      uint16_t c = readWord(wordAddress);
      uint16_t d = readWord(wordAddress);
      if (((c ^ d) & 0x0040) == 0) return true;
      return false;
    }
  }
  return false;
}

bool MX26L6420SocketDevice::erase() {
  clearFlashError(error);
  writeWord(UNLOCK1, 0x00AA);
  writeWord(UNLOCK2, 0x0055);
  writeWord(UNLOCK1, 0x0080);
  writeWord(UNLOCK1, 0x00AA);
  writeWord(UNLOCK2, 0x0055);
  writeWord(UNLOCK1, 0x0010);

  bool ok = waitToggleReady(0x0000UL, 0xFFFF, 300000UL);
  reset();
  if (!ok) error.code = 'E';
  return ok;
}

bool MX26L6420SocketDevice::writeBlock(uint32_t byteOffset, uint16_t pageSizeBytes,
                                       uint32_t blockSizeBytes, const byte *data) {
  clearFlashError(error);
  if (!pageSizeValid(pageSizeBytes) || (byteOffset & 1U) || (blockSizeBytes & 1U) ||
      blockSizeBytes == 0 || byteOffset + blockSizeBytes > SIZE_BYTES) {
    error.code = 'B';
    error.byteOffset = byteOffset;
    return false;
  }

  for (uint32_t bi = 0; bi < blockSizeBytes; bi += 2) {
    uint32_t wordAddr = (byteOffset + bi) / 2;
    uint16_t wordData = ((data[bi + 1] & 0xFF) << 8) | (data[bi] & 0xFF);

    writeWord(UNLOCK1, 0x00AA);
    writeWord(UNLOCK2, 0x0055);
    writeWord(UNLOCK1, 0x00A0);
    writeWord(wordAddr, wordData);

    if (!waitToggleReady(wordAddr, wordData, 1000UL)) {
      error.code = 'P';
      error.byteOffset = byteOffset + bi;
      error.expected = wordData;
      error.actual = readWord(wordAddr);
      reset();
      return false;
    }
  }
  dataIn();
  return true;
}

uint32_t MX26L6420SocketDevice::crc32(uint32_t byteOffset, uint32_t byteCount) {
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
