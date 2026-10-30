#ifndef ARDUINO_H
#define ARDUINO_H
#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
using byte = uint8_t;
using word = uint16_t;
#define F(x) x
extern uint8_t DDRF, DDRK, DDRL, DDRH, DDRC, DDRA;
extern uint8_t PORTF, PORTK, PORTL, PORTH, PORTC, PORTA;
extern uint8_t PINC, PINA;
unsigned long millis();
void delay(unsigned long);
void delayMicroseconds(unsigned int);
class SerialStub {
public:
  void begin(unsigned long) {}
  int available() { return 0; }
  int read() { return -1; }
  void flush() {}
  size_t write(uint8_t) { return 1; }
  size_t write(const uint8_t*, size_t n) { return n; }
  void print(const char*) {}
  void print(char) {}
  void print(uint8_t) {}
  void print(uint16_t) {}
  void print(uint32_t) {}
  void print(unsigned long) {}
  void print(long) {}
  void print(int) {}
  void println(const char*) {}
  void println(uint8_t) {}
  void println(uint16_t) {}
  void println(uint32_t) {}
  void println(unsigned long) {}
  void println(int) {}
  void println() {}
};
extern SerialStub Serial;
#endif
