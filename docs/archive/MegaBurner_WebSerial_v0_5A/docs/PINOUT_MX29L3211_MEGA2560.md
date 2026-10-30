# Pinout MX29L3211 SOP44 ↔ Arduino Mega 2560

Este pinout consolida el cableado usado por MegaBurner/cartreader y el firmware de este port. La memoria debe operar a 3.3 V.

## Control

| MX29L3211 pin | Señal MX29L3211 | Arduino Mega pin | Puerto AVR | Nota |
|---:|---|---:|---|---|
| 1 | WE# | D7 | PH4 | Write Enable |
| 12 | CE# | D9 | PH6 | Chip Enable |
| 14 | OE# | D16 | PH1 | Output Enable |
| 33 | BYTE# | D6 / VCC | PH3 | Mantener HIGH para modo 16-bit |

## Dirección

| MX29L3211 pin | Señal | Arduino Mega pin | Puerto AVR |
|---:|---|---:|---|
| 11 | A0 | A0 / D54 | PF0 |
| 10 | A1 | A1 / D55 | PF1 |
| 9 | A2 | A2 / D56 | PF2 |
| 8 | A3 | A3 / D57 | PF3 |
| 7 | A4 | A4 / D58 | PF4 |
| 6 | A5 | A5 / D59 | PF5 |
| 5 | A6 | A6 / D60 | PF6 |
| 4 | A7 | A7 / D61 | PF7 |
| 42 | A8 | A8 / D62 | PK0 |
| 41 | A9 | A9 / D63 | PK1 |
| 40 | A10 | A10 / D64 | PK2 |
| 39 | A11 | A11 / D65 | PK3 |
| 37 | A12 | A12 / D66 | PK4 |
| 36 | A13 | A13 / D67 | PK5 |
| 35 | A14 | A14 / D68 | PK6 |
| 34 | A16 | D49 | PL0 |
| 3 | A17 | D48 | PL1 |
| 2 | A18 | D47 | PL2 |
| 43 | A19 | D46 | PL3 |
| 44 | A20 | D45 | PL4 |
| 38 | A15 | A15 / D69 | PK7 |

## Datos 16-bit

| MX29L3211 pin | Señal | Arduino Mega pin | Puerto AVR |
|---:|---|---:|---|
| 30 | Q0 | D37 | PC0 |
| 28 | Q1 | D36 | PC1 |
| 26 | Q2 | D35 | PC2 |
| 24 | Q3 | D34 | PC3 |
| 21 | Q4 | D33 | PC4 |
| 19 | Q5 | D32 | PC5 |
| 17 | Q6 | D31 | PC6 |
| 15 | Q7 | D30 | PC7 |
| 31 | Q8 | D22 | PA0 |
| 29 | Q9 | D23 | PA1 |
| 27 | Q10 | D24 | PA2 |
| 25 | Q11 | D25 | PA3 |
| 22 | Q12 | D26 | PA4 |
| 20 | Q13 | D27 | PA5 |
| 18 | Q14 | D28 | PA6 |
| 16 | Q15 | D29 | PA7 |

## Alimentación

| MX29L3211 pin | Señal | Conectar a |
|---:|---|---|
| 13 | VCC | 3.3 V |
| 38 | VCC | 3.3 V |
| 23 | GND | GND |
| 32 | GND | GND |

## Nota para 64 Mbit futuro

En la MX29L3211 SOP44, el pin físico 44 es A20. Para chips de 64 Mbit x16 se requeriría A21; en el Arduino Mega la candidata lógica sería D44/PL5, pero debe validarse contra el datasheet del chip específico y el adaptador físico.
