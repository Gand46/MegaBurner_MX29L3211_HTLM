/**
 * MX29L3211 - MegaBurner WebSerial optimized branch
 * Base version 1.0, 2017.11.11
 * v0.5E Final500K: status-aware busy polling + fast CRC32 table.
 */
#include "MX29L3211.h"
#include <avr/pgmspace.h>

MX29L3211::MX29L3211() {}

void MX29L3211::init()
{
  // Set Address Pins to Output: A0-A7, A8-A15, A16-A23
  DDRF = 0xFF;
  DDRK = 0xFF;
  DDRL = 0xFF;

  // Set Control Pins to Output OE(PH1) BYTE(PH3) WE(PH4) CE(PH6)
  DDRH |= (1 << 1) | (1 << 3) | (1 << 4) | (1 << 6);

  // Set Data Pins (D0-D15) to Input and disable internal pullups
  DDRC = 0x00;
  DDRA = 0x00;
  PORTC = 0x00;
  PORTA = 0x00;

  // OE, BYTE, WE HIGH; CE LOW
  PORTH |= (1 << 1) | (1 << 3) | (1 << 4);
  PORTH &= ~(1 << 6);

  delay(100);
  readId();
  reset();
}

void MX29L3211::readId() {
  dataOut();
  writeWord(0x5555, 0xaa);
  writeWord(0x2aaa, 0x55);
  writeWord(0x5555, 0x90);
  dataIn();

  for (int i = 0; i < 2; i++) {
    byte b = readByte(i);
    if (b < 0x10) Serial.print('0');
    Serial.print(b, HEX);
  }
}

void MX29L3211::reset() {
  dataOut();
  writeWord(0x5555, 0xaa);
  writeWord(0x2aaa, 0x55);
  writeWord(0x5555, 0xf0);
  dataIn();
  delay(500);
}

word MX29L3211::status() {
  word sr = readStatusReg();
  dataIn();
  return sr;
}

void MX29L3211::clearStatus() {
  clearStatusReg();
}

void MX29L3211::read8(long block_id, long block_size) {
  long start = (block_id * block_size) / 2;
  for (long i = start; i < (start + block_size); i++) {
    Serial.write(readByte(i));
  }
}

void MX29L3211::read16(long block_id, long block_size) {
  long start = (block_id * block_size) / 2;
  for (long i = start; i < start + block_size / 2; i++) {
    word aword = readWord(i);
    Serial.write(aword & 0xFF);
    Serial.write((aword >> 8) & 0xFF);
  }
}

// CRC32 nibble table in flash, not SRAM.
static const uint32_t crc32_tab16[] PROGMEM = {
  0x00000000UL, 0x1DB71064UL, 0x3B6E20C8UL, 0x26D930ACUL,
  0x76DC4190UL, 0x6B6B51F4UL, 0x4DB26158UL, 0x5005713CUL,
  0xEDB88320UL, 0xF00F9344UL, 0xD6D6A3E8UL, 0xCB61B38CUL,
  0x9B64C2B0UL, 0x86D3D2D4UL, 0xA00AE278UL, 0xBDBDF21CUL
};

static inline uint32_t mx_crc32_update_nibble(uint32_t crc, byte data) {
  crc ^= data;
  crc = (crc >> 4) ^ pgm_read_dword(&crc32_tab16[crc & 0x0F]);
  crc = (crc >> 4) ^ pgm_read_dword(&crc32_tab16[crc & 0x0F]);
  return crc;
}

uint32_t MX29L3211::crc32_16(uint32_t offset, uint32_t byteCount) {
  uint32_t crc = 0xFFFFFFFFUL;
  uint32_t wordAddress = offset / 2;
  bool oddStart = (offset & 1) != 0;
  uint32_t processed = 0;

  dataIn();
  while (processed < byteCount) {
    word w = readWord(wordAddress++);
    byte lo = w & 0xFF;
    byte hi = (w >> 8) & 0xFF;

    if (oddStart) {
      crc = mx_crc32_update_nibble(crc, hi);
      processed++;
      oddStart = false;
      continue;
    }

    crc = mx_crc32_update_nibble(crc, lo);
    processed++;
    if (processed < byteCount) {
      crc = mx_crc32_update_nibble(crc, hi);
      processed++;
    }
  }
  return crc ^ 0xFFFFFFFFUL;
}

uint8_t MX29L3211::eraseStatus() {
  dataOut();
  writeWord(0x5555, 0xaa);
  writeWord(0x2aaa, 0x55);
  writeWord(0x5555, 0x80);
  writeWord(0x5555, 0xaa);
  writeWord(0x2aaa, 0x55);
  writeWord(0x5555, 0x10);
  dataIn();

  uint8_t res = waitReady(ERASE_TIMEOUT_MS);
  dataIn();
  return res;
}

void MX29L3211::erase() {
  (void)eraseStatus();
}

uint8_t MX29L3211::write8Status(long offset, long page_size, long block_size, byte data[]) {
  dataOut();
  for (long bi = 0; bi < block_size; bi += page_size) {
    delayMicroseconds(100);
    uint8_t ready = waitReady(PROGRAM_TIMEOUT_MS);
    if (ready != MB_FLASH_OK) { dataIn(); return ready; }

    writeWord(0x5555, 0xaa);
    writeWord(0x2aaa, 0x55);
    writeWord(0x5555, 0xa0);

    long thisPage = page_size;
    if (bi + thisPage > block_size) thisPage = block_size - bi;
    for (long pi = 0; pi < thisPage; pi++) {
      writeByte(offset + bi + pi, data[bi + pi]);
    }
  }

  delayMicroseconds(100);
  uint8_t res = waitReady(PROGRAM_TIMEOUT_MS);
  dataIn();
  return res;
}

void MX29L3211::write8(long offset, long page_size, long block_size, byte data[]) {
  (void)write8Status(offset, page_size, block_size, data);
}

uint8_t MX29L3211::write16Status(long offset, long page_size, long block_size, byte data[]) {
  dataOut();
  for (long bi = 0; bi < block_size / 2; bi += page_size / 2) {
    delayMicroseconds(100);
    uint8_t ready = waitReady(PROGRAM_TIMEOUT_MS);
    if (ready != MB_FLASH_OK) { dataIn(); return ready; }

    writeWord(0x5555, 0xaa);
    writeWord(0x2aaa, 0x55);
    writeWord(0x5555, 0xa0);

    long thisPageWords = page_size / 2;
    long remainingWords = (block_size / 2) - bi;
    if (thisPageWords > remainingWords) thisPageWords = remainingWords;

    for (long pi = 0; pi < thisPageWords; pi++) {
      long address = offset / 2 + bi + pi;
      long a = (bi + pi) * 2;
      long b = a + 1;
      word currWord = ((data[b] & 0xFF) << 8) | (data[a] & 0xFF);
      writeWord(address, currWord);
    }
  }

  delayMicroseconds(100);
  uint8_t res = waitReady(PROGRAM_TIMEOUT_MS);
  dataIn();
  return res;
}

void MX29L3211::write16(long offset, long page_size, long block_size, byte data[]) {
  (void)write16Status(offset, page_size, block_size, data);
}
