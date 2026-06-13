# MX29L3211 SOP44 to Arduino Mega 2560 pinout

This table follows the MX29L3211 44-SOP pin names and the direct-port mapping used by the MegaBurner firmware.

| MX29L3211 SOP44 pin | Signal | Group | Arduino Mega 2560 pin | ATmega2560 port bit | Notes |
|---:|---|---|---|---|---|
| 1 | WE | Control | D7 | PH4 | Write Enable |
| 2 | A18 | Address | D47 | PL2 | Address bit 18 |
| 3 | A17 | Address | D48 | PL1 | Address bit 17 |
| 4 | A7 | Address | A7 / D61 | PF7 | Address bit 7 |
| 5 | A6 | Address | A6 / D60 | PF6 | Address bit 6 |
| 6 | A5 | Address | A5 / D59 | PF5 | Address bit 5 |
| 7 | A4 | Address | A4 / D58 | PF4 | Address bit 4 |
| 8 | A3 | Address | A3 / D57 | PF3 | Address bit 3 |
| 9 | A2 | Address | A2 / D56 | PF2 | Address bit 2 |
| 10 | A1 | Address | A1 / D55 | PF1 | Address bit 1 |
| 11 | A0 | Address | A0 / D54 | PF0 | Address bit 0 |
| 12 | CE | Control | D9 | PH6 | Chip Enable |
| 13 | GND | Power | GND | — | Ground |
| 14 | OE | Control | D16 | PH1 | Output Enable |
| 15 | Q0 | Data low | D37 | PC0 | Data bit 0 |
| 16 | Q8 | Data high | D22 | PA0 | Data bit 8 |
| 17 | Q1 | Data low | D36 | PC1 | Data bit 1 |
| 18 | Q9 | Data high | D23 | PA1 | Data bit 9 |
| 19 | Q2 | Data low | D35 | PC2 | Data bit 2 |
| 20 | Q10 | Data high | D24 | PA2 | Data bit 10 |
| 21 | Q3 | Data low | D34 | PC3 | Data bit 3 |
| 22 | Q11 | Data high | D25 | PA3 | Data bit 11 |
| 23 | VCC | Power | 3.3V regulated | — | MX29L3211 VCC, not 5V |
| 24 | Q4 | Data low | D33 | PC4 | Data bit 4 |
| 25 | Q12 | Data high | D26 | PA4 | Data bit 12 |
| 26 | Q5 | Data low | D32 | PC5 | Data bit 5 |
| 27 | Q13 | Data high | D27 | PA5 | Data bit 13 |
| 28 | Q6 | Data low | D31 | PC6 | Data bit 6 |
| 29 | Q14 | Data high | D28 | PA6 | Data bit 14 |
| 30 | Q7 | Data low | D30 | PC7 | Data bit 7 |
| 31 | Q15/A-1 | Data high | D29 | PA7 | Data bit 15 in word mode |
| 32 | GND | Power | GND | — | Ground |
| 33 | BYTE | Control | D6 | PH3 | Held HIGH for x16 word mode |
| 34 | A16 | Address | D49 | PL0 | Address bit 16 |
| 35 | A15 | Address | A15 / D69 | PK7 | Address bit 15 |
| 36 | A14 | Address | A14 / D68 | PK6 | Address bit 14 |
| 37 | A13 | Address | A13 / D67 | PK5 | Address bit 13 |
| 38 | A12 | Address | A12 / D66 | PK4 | Address bit 12 |
| 39 | A11 | Address | A11 / D65 | PK3 | Address bit 11 |
| 40 | A10 | Address | A10 / D64 | PK2 | Address bit 10 |
| 41 | A9 | Address | A9 / D63 | PK1 | Address bit 9 |
| 42 | A8 | Address | A8 / D62 | PK0 | Address bit 8 |
| 43 | A19 | Address | D46 | PL3 | Address bit 19 |
| 44 | A20 | Address | D45 | PL4 | Address bit 20 |


## Future 64-Mbit note

The MX29L3211 SOP44 **physical pin 44 is A20**. In this project conversation, the likely future expansion point called “pin 44” refers to **Arduino Mega digital pin D44 / ATmega2560 PL5**, which is available in the current port layout and can be assigned as an extra address line **A21** for a future 64-Mbit x16 flash, if the target chip/adaptor exposes A21.

Current address-line usage:

| Address line | Arduino Mega pin | Port bit |
|---|---|---|
| A16 | D49 | PL0 |
| A17 | D48 | PL1 |
| A18 | D47 | PL2 |
| A19 | D46 | PL3 |
| A20 | D45 | PL4 |
| Future A21 | D44 | PL5 |
| Future A22 | D43 | PL6 |
| Future A23 | D42 | PL7 |

A future 64-Mbit implementation must be validated against the exact flash datasheet. Do not assume pin compatibility from capacity alone.
