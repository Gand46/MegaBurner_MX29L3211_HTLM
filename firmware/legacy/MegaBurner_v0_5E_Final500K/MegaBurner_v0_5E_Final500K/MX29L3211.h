/**
 * MX29L3211 - MegaBurner WebSerial optimized branch
 * Base version 1.0, 2017.11.11
 * v0.5E Final500K: status-aware busy polling + fast CRC32 table.
 */
#ifndef MX29L3211_h
#define MX29L3211_h

#include "Arduino.h"

#define MB_FLASH_OK            0
#define MB_FLASH_PROGRAM_FAIL  1
#define MB_FLASH_ERASE_FAIL    2
#define MB_FLASH_TIMEOUT       3

class MX29L3211 {

  public:
    char id[4];
    MX29L3211();
    void init();
    void readId();
    void reset();
    void read8(long block_id, long block_size);
    void read16(long block_id, long block_size);

    // Legacy-compatible wrappers.
    void write8(long offset, long page_size, long block_size, byte data[]);
    void write16(long offset, long page_size, long block_size, byte data[]);
    void erase();

    // v0.5E status-aware operations.
    uint8_t write8Status(long offset, long page_size, long block_size, byte data[]);
    uint8_t write16Status(long offset, long page_size, long block_size, byte data[]);
    uint8_t eraseStatus();
    uint32_t crc32_16(uint32_t offset, uint32_t byteCount);
    word status();
    void clearStatus();

  private:
    // Conservative timeouts. Page program is typically much shorter, chip erase can be long.
    static const unsigned long PROGRAM_TIMEOUT_MS = 2000UL;
    static const unsigned long ERASE_TIMEOUT_MS   = 180000UL;

    // Switch data pins to write
    void dataOut() {
      DDRC = 0xFF;
      DDRA = 0xFF;
    }

    // Switch data pins to read
    void dataIn() {
      DDRC = 0x00;
      DDRA = 0x00;
    }

    void writeByte(unsigned long address, byte data) {
      PORTF = address & 0xFF;
      PORTK = (address >> 8) & 0xFF;
      PORTL = (address >> 16) & 0xFF;
      PORTC = data;

      // Arduino running at 16Mhz -> one nop = 62.5ns
      __asm__("nop\n\t");
      PORTH &= ~(1 << 4);       // WE LOW
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
      PORTH |= (1 << 4);        // WE HIGH
      __asm__("nop\n\t");
    }

    void writeWord(unsigned long address, word data) {
      PORTF = address & 0xFF;
      PORTK = (address >> 8) & 0xFF;
      PORTL = (address >> 16) & 0xFF;
      PORTC = data;
      PORTA = (data >> 8) & 0xFF;

      // Arduino running at 16Mhz -> one nop = 62.5ns
      __asm__("nop\n\t");
      PORTH &= ~(1 << 4);       // WE LOW
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
      PORTH |= (1 << 4);        // WE HIGH
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    }

    byte readByte(unsigned long address) {
      PORTF = address & 0xFF;
      PORTK = (address >> 8) & 0xFF;
      PORTL = (address >> 16) & 0xFF;
      __asm__("nop\n\t");
      PORTH &= ~(1 << 1);       // OE LOW
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
      byte tempByte = PINC;
      PORTH |= (1 << 1);        // OE HIGH
      __asm__("nop\n\t");
      return tempByte;
    }

    word readWord(unsigned long address) {
      PORTF = address & 0xFF;
      PORTK = (address >> 8) & 0xFF;
      PORTL = (address >> 16) & 0xFF;
      __asm__("nop\n\t");
      PORTH &= ~(1 << 1);       // OE LOW
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
      word tempWord = ((PINA & 0xFF) << 8) | (PINC & 0xFF);
      __asm__("nop\n\t");
      PORTH |= (1 << 1);        // OE HIGH
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
      return tempWord;
    }

    word readStatusReg() {
      dataOut();
      writeWord(0x5555, 0xaa);
      writeWord(0x2aaa, 0x55);
      writeWord(0x5555, 0x70);
      dataIn();
      return readWord(0);
    }

    void clearStatusReg() {
      dataOut();
      writeWord(0x5555, 0xaa);
      writeWord(0x2aaa, 0x55);
      writeWord(0x5555, 0x50);
      dataOut();
    }

    uint8_t waitReady(unsigned long timeoutMs) {
      unsigned long started = millis();
      word sr = readStatusReg();
      while ((sr & 0x0080) == 0) {  // Q7: 1 = ready, 0 = busy
        if ((sr & 0x0020) != 0) {   // Q5 erase fail
          clearStatusReg();
          dataOut();
          return MB_FLASH_ERASE_FAIL;
        }
        if ((sr & 0x0010) != 0) {   // Q4 program fail
          clearStatusReg();
          dataOut();
          return MB_FLASH_PROGRAM_FAIL;
        }
        if (millis() - started > timeoutMs) {
          dataOut();
          return MB_FLASH_TIMEOUT;
        }
        sr = readWord(0);           // Still in Read Status Register mode.
      }

      if ((sr & 0x0020) != 0) {
        clearStatusReg();
        dataOut();
        return MB_FLASH_ERASE_FAIL;
      }
      if ((sr & 0x0010) != 0) {
        clearStatusReg();
        dataOut();
        return MB_FLASH_PROGRAM_FAIL;
      }
      dataOut();
      return MB_FLASH_OK;
    }
};

#endif
