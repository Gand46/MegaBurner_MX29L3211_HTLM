# Comparación de pines SOP44

## Pines que cambian de función

| Pin | MX29L3211 | MX26L6420 | MX29LV160CT | Mega 2560 |
|---:|---|---|---|---|
| 1 | WE# | A21 | RESET# | D7 / PH4 |
| 33 | BYTE# | WE# | BYTE# | D6 / PH3 |
| 44 | A20 | A20 | WE# | D45 / PL4 |

## Pines comunes principales

| Función | Pin SOP44 | Mega 2560 |
|---|---:|---|
| CE# | 12 | D9 / PH6 |
| OE# | 14 | D16 / PH1 |
| A0–A7 | 11–4 | A0–A7 / PF0–PF7 |
| A8–A15 | 42–35 | A8–A15 / PK0–PK7 |
| A16 | 34 | D49 / PL0 |
| A17 | 3 | D48 / PL1 |
| A18 | 2 | D47 / PL2 |
| A19 | 43 | D46 / PL3 |
| Q0–Q7 | 15,17,19,21,24,26,28,30 | D37–D30 / PC0–PC7 |
| Q8–Q15 | 16,18,20,22,25,27,29,31 | D22–D29 / PA0–PA7 |

## Consecuencia en firmware

- MX29L3211 pulsa WE en PH4 y usa PL4 como A20.
- MX26L6420 usa PH4 como A21, pulsa WE en PH3 y usa PL4 como A20.
- MX29LV160CT mantiene PH4 como RESET#, PH3 como BYTE# y pulsa WE en PL4; solo PL0–PL3 transportan A16–A19.
