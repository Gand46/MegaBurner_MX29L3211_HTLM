#include "MX29L3211Device.h"

MX29L3211Device::MX29L3211Device() {
  clearFlashError(error);
}

void MX29L3211Device::activate() {
  clearFlashError(error);

  DDRF = 0xFF;  // A0-A7
  DDRK = 0xFF;  // A8-A15
  DDRL = 0xFF;  // A16-A23 (A16-A20 used)

  // OE(PH1), BYTE(PH3), WE(PH4), CE(PH6)
  DDRH |= (1 << 1) | (1 << 3) | (1 << 4) | (1 << 6);

  DDRC = 0x00;
  DDRA = 0x00;
  PORTC = 0x00;
  PORTA = 0x00;

  PORTH |= (1 << 1) | (1 << 3) | (1 << 4); // OE/BYTE/WE high
  PORTH &= ~(1 << 6);                       // CE low
  delay(10);
}

void MX29L3211Device::deactivate() {
  PORTH |= (1 << 1) | (1 << 3) | (1 << 4) | (1 << 6);
  DDRC = 0x00;
  DDRA = 0x00;
  PORTC = 0x00;
  PORTA = 0x00;
}

void MX29L3211Device::writeWord(uint32_t wordAddress, uint16_t data) {
  PORTF = wordAddress & 0xFF;
  PORTK = (wordAddress >> 8) & 0xFF;
  PORTL = (wordAddress >> 16) & 0xFF;
  PORTC = (byte)(data & 0xFF);
  PORTA = (byte)((data >> 8) & 0xFF);

  __asm__("nop\n\t");
  PORTH &= ~(1 << 4); // WE low
  __asm__("nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t");
  PORTH |= (1 << 4);  // WE high
  __asm__("nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t");
}

uint16_t MX29L3211Device::readWord(uint32_t wordAddress) {
  PORTF = wordAddress & 0xFF;
  PORTK = (wordAddress >> 8) & 0xFF;
  PORTL = (wordAddress >> 16) & 0xFF;

  __asm__("nop\n\t");
  PORTH &= ~(1 << 1); // OE low
  __asm__("nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t");
  uint16_t value = ((PINA & 0xFF) << 8) | (PINC & 0xFF);
  __asm__("nop\n\t");
  PORTH |= (1 << 1);  // OE high
  __asm__("nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t");
  return value;
}

byte MX29L3211Device::readByte(uint32_t byteAddress) {
  PORTF = byteAddress & 0xFF;
  PORTK = (byteAddress >> 8) & 0xFF;
  PORTL = (byteAddress >> 16) & 0xFF;
  __asm__("nop\n\t");
  PORTH &= ~(1 << 1);
  __asm__("nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t");
  byte value = PINC;
  PORTH |= (1 << 1);
  __asm__("nop\n\t");
  return value;
}

void MX29L3211Device::reset() {
  dataOut();
  writeWord(0x5555UL, 0x00AA);
  writeWord(0x2AAAUL, 0x0055);
  writeWord(0x5555UL, 0x00F0);
  dataIn();
  delay(500);
}

bool MX29L3211Device::readId(uint16_t &manufacturer, uint16_t &device) {
  dataOut();
  writeWord(0x5555UL, 0x00AA);
  writeWord(0x2AAAUL, 0x0055);
  writeWord(0x5555UL, 0x0090);
  dataIn();

  manufacturer = (uint16_t)readByte(0);
  device = (uint16_t)readByte(1);
  reset();
  return manufacturer == MANUFACTURER_ID && device == DEVICE_ID;
}

void MX29L3211Device::readBytes(uint32_t byteOffset, uint32_t byteCount) {
  uint32_t startWord = byteOffset / 2;
  uint32_t words = byteCount / 2;
  dataIn();
  for (uint32_t i = 0; i < words; i++) {
    uint16_t w = readWord(startWord + i);
    Serial.write((byte)(w & 0xFF));
    Serial.write((byte)((w >> 8) & 0xFF));
  }
}

uint16_t MX29L3211Device::readStatusReg() {
  dataOut();
  writeWord(0x5555UL, 0x00AA);
  writeWord(0x2AAAUL, 0x0055);
  writeWord(0x5555UL, 0x0070);
  dataIn();
  return readWord(0);
}

void MX29L3211Device::busyCheck() {
  uint16_t statusReg = readStatusReg();
  while ((statusReg | 0xFF7F) != 0xFFFF) {
    statusReg = readWord(0);
  }
  dataOut();
}

bool MX29L3211Device::erase() {
  clearFlashError(error);
  dataOut();
  writeWord(0x5555UL, 0x00AA);
  writeWord(0x2AAAUL, 0x0055);
  writeWord(0x5555UL, 0x0080);
  writeWord(0x5555UL, 0x00AA);
  writeWord(0x2AAAUL, 0x0055);
  writeWord(0x5555UL, 0x0010);
  dataIn();
  busyCheck();
  return true;
}

bool MX29L3211Device::pageSizeValid(uint16_t pageSizeBytes) const {
  return pageSizeBytes == 128 || pageSizeBytes == 256;
}

bool MX29L3211Device::writeBlock(uint32_t byteOffset, uint16_t pageSizeBytes,
                                  uint32_t blockSizeBytes, const byte *data) {
  clearFlashError(error);
  if (!pageSizeValid(pageSizeBytes) || blockSizeBytes == 0 ||
      (byteOffset & 1U) || (blockSizeBytes & 1U) ||
      byteOffset + blockSizeBytes > SIZE_BYTES) {
    error.code = 'B';
    error.byteOffset = byteOffset;
    return false;
  }
  if (blockSizeBytes < pageSizeBytes) pageSizeBytes = (uint16_t)blockSizeBytes;

  dataOut();
  for (uint32_t bi = 0; bi < blockSizeBytes / 2; bi += pageSizeBytes / 2) {
    delayMicroseconds(100);
    busyCheck();

    writeWord(0x5555UL, 0x00AA);
    writeWord(0x2AAAUL, 0x0055);
    writeWord(0x5555UL, 0x00A0);

    for (uint32_t pi = 0; pi < pageSizeBytes / 2; pi++) {
      uint32_t address = byteOffset / 2 + bi + pi;
      uint32_t dataIndex = (bi + pi) * 2;
      uint16_t wordData = ((data[dataIndex + 1] & 0xFF) << 8) | (data[dataIndex] & 0xFF);
      writeWord(address, wordData);
    }
  }

  delayMicroseconds(100);
  busyCheck();
  dataIn();
  return true;
}

uint32_t MX29L3211Device::crc32(uint32_t byteOffset, uint32_t byteCount) {
  uint32_t crc = 0xFFFFFFFFUL;
  uint32_t wordAddress = byteOffset / 2;
  bool oddStart = (byteOffset & 1U) != 0;
  uint32_t processed = 0;
  dataIn();

  while (processed < byteCount) {
    uint16_t w = readWord(wordAddress++);
    byte lo = (byte)(w & 0xFF);
    byte hi = (byte)((w >> 8) & 0xFF);
    if (oddStart) {
      crc = flashCrc32Update(crc, hi);
      processed++;
      oddStart = false;
      continue;
    }
    crc = flashCrc32Update(crc, lo);
    processed++;
    if (processed < byteCount) {
      crc = flashCrc32Update(crc, hi);
      processed++;
    }
  }
  return crc ^ 0xFFFFFFFFUL;
}
