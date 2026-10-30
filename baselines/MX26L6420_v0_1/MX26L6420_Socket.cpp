#include "MX26L6420_Socket.h"

MX26L6420Socket::MX26L6420Socket() {}

void MX26L6420Socket::init() {
  // Address pins:
  // A0-A7  -> PORTF
  // A8-A15 -> PORTK
  // A16-A20 -> PORTL bits 0-4
  // A21 -> PORTH bit 4 due to MX29L3211-compatible socket pin swap.
  DDRF = 0xFF;
  DDRK = 0xFF;
  DDRL |= 0x1F;

  // Control/address pins on PORTH:
  // OE(PH1), WE(PH3), A21(PH4), CE(PH6)
  DDRH |= (1 << OE_BIT) | (1 << WE_BIT) | (1 << A21_BIT) | (1 << CE_BIT);

  dataIn();
  PORTC = 0x00;
  PORTA = 0x00;

  // Safe idle state:
  // OE high, WE high, A21 low, CE low.
  PORTH |= (1 << OE_BIT) | (1 << WE_BIT);
  PORTH &= ~(1 << A21_BIT);
  PORTH &= ~(1 << CE_BIT);

  setAddress(0);
  delay(100);
  reset();
}

void MX26L6420Socket::writeWord(uint32_t wordAddress, uint16_t data) {
  dataOut();
  setAddress(wordAddress);
  PORTC = data & 0xFF;
  PORTA = (data >> 8) & 0xFF;

  // Address/data setup. AVR @ 16 MHz: 1 nop ≈ 62.5 ns.
  __asm__("nop\n\t" "nop\n\t");

  // Pulse WE on PH3, not PH4. PH4 is A21 in this socket-compatible firmware.
  PORTH &= ~(1 << WE_BIT);
  __asm__("nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t");
  PORTH |= (1 << WE_BIT);
  __asm__("nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t");
}

uint16_t MX26L6420Socket::readWord(uint32_t wordAddress) {
  setAddress(wordAddress);
  __asm__("nop\n\t" "nop\n\t");

  PORTH &= ~(1 << OE_BIT); // OE low
  __asm__("nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t" "nop\n\t");

  uint16_t value = ((PINA & 0xFF) << 8) | (PINC & 0xFF);

  PORTH |= (1 << OE_BIT);  // OE high
  __asm__("nop\n\t" "nop\n\t" "nop\n\t");
  return value;
}

void MX26L6420Socket::reset() {
  // MX26L6420 uses AMD/JEDEC-style read/reset command. Single-cycle F0 is kept
  // intentionally simple and avoids applying MX29L3211 page-program behavior.
  writeWord(0x0000UL, 0x00F0);
  dataIn();
  delay(10);
}

void MX26L6420Socket::readId(uint16_t *manufacturer, uint16_t *device) {
  writeWord(UNLOCK1, 0x00AA);
  writeWord(UNLOCK2, 0x0055);
  writeWord(UNLOCK1, 0x0090);
  dataIn();
  *manufacturer = readWord(0x0000UL);
  *device = readWord(0x0001UL);
  reset();
}

void MX26L6420Socket::read16(uint32_t blockId, uint32_t blockSizeBytes) {
  uint32_t startWord = (blockId * blockSizeBytes) / 2;
  uint32_t words = blockSizeBytes / 2;
  dataIn();
  for (uint32_t i = 0; i < words; i++) {
    uint16_t w = readWord(startWord + i);
    Serial.write(w & 0xFF);
    Serial.write((w >> 8) & 0xFF);
  }
}

bool MX26L6420Socket::waitToggleReady(uint32_t wordAddress, uint16_t expected, uint32_t timeoutMs) {
  dataIn();
  unsigned long start = millis();
  while (millis() - start < timeoutMs) {
    uint16_t a = readWord(wordAddress);
    uint16_t b = readWord(wordAddress);

    // DQ6 toggle polling: when DQ6 stops toggling, the operation has completed.
    if (((a ^ b) & 0x0040) == 0) {
      uint16_t finalValue = readWord(wordAddress);
      // For program, exact match is expected. For erase, expected=FFFF.
      return (finalValue == expected) || ((finalValue & 0x0080) == (expected & 0x0080));
    }

    // DQ5 is a timeout/fail hint. Re-read toggle before declaring failure.
    if (b & 0x0020) {
      uint16_t c = readWord(wordAddress);
      uint16_t d = readWord(wordAddress);
      if (((c ^ d) & 0x0040) == 0) return true;
      return false;
    }
  }
  return false;
}

bool MX26L6420Socket::erase() {
  writeWord(UNLOCK1, 0x00AA);
  writeWord(UNLOCK2, 0x0055);
  writeWord(UNLOCK1, 0x0080);
  writeWord(UNLOCK1, 0x00AA);
  writeWord(UNLOCK2, 0x0055);
  writeWord(UNLOCK1, 0x0010);

  bool ok = waitToggleReady(0x0000UL, 0xFFFF, 240000UL);
  reset();
  return ok;
}

bool MX26L6420Socket::write16WordBlock(uint32_t byteOffset, uint32_t blockSizeBytes, const byte data[]) {
  if ((byteOffset & 1) || (blockSizeBytes & 1)) return false;
  if (byteOffset + blockSizeBytes > SIZE_BYTES) return false;

  for (uint32_t bi = 0; bi < blockSizeBytes; bi += 2) {
    uint32_t wordAddr = (byteOffset + bi) / 2;
    uint16_t wordData = ((data[bi + 1] & 0xFF) << 8) | (data[bi] & 0xFF);

    // Skip erased words only when the HTML already skipped whole FF blocks/pages.
    // Here every received word is intentionally programmed for deterministic behavior.
    writeWord(UNLOCK1, 0x00AA);
    writeWord(UNLOCK2, 0x0055);
    writeWord(UNLOCK1, 0x00A0);
    writeWord(wordAddr, wordData);

    if (!waitToggleReady(wordAddr, wordData, 1000UL)) {
      reset();
      return false;
    }
  }
  dataIn();
  return true;
}

uint32_t MX26L6420Socket::crc32_update(uint32_t crc, byte data) {
  crc ^= data;
  for (byte i = 0; i < 8; i++) {
    crc = (crc & 1) ? (crc >> 1) ^ 0xEDB88320UL : (crc >> 1);
  }
  return crc;
}

uint32_t MX26L6420Socket::crc32_16(uint32_t byteOffset, uint32_t byteCount) {
  uint32_t crc = 0xFFFFFFFFUL;
  uint32_t startWord = byteOffset / 2;
  uint32_t words = byteCount / 2;
  dataIn();
  for (uint32_t i = 0; i < words; i++) {
    uint16_t w = readWord(startWord + i);
    crc = crc32_update(crc, w & 0xFF);
    crc = crc32_update(crc, (w >> 8) & 0xFF);
  }
  return ~crc;
}
