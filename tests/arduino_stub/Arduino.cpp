#include "Arduino.h"
uint8_t DDRF=0, DDRK=0, DDRL=0, DDRH=0, DDRC=0, DDRA=0;
uint8_t PORTF=0, PORTK=0, PORTL=0, PORTH=0, PORTC=0, PORTA=0;
uint8_t PINC=0, PINA=0;
SerialStub Serial;
unsigned long millis(){ static unsigned long t=0; return ++t; }
void delay(unsigned long){}
void delayMicroseconds(unsigned int){}
