# Trazabilidad de las ramas base

## MX29L3211

La carpeta `baselines/MX29L3211_v0_5A/` contiene la versión subida por el usuario. El driver unificado mantiene:

- WE en PH4.
- BYTE en PH3.
- A20 en PL4.
- Unlock `5555/2AAA`.
- Page Program 256/128.
- FastWrite S/X/Z.
- CRC y lectura de bloques.

## MX26L6420

La carpeta `baselines/MX26L6420_v0_1/` contiene la versión subida y validada por el usuario. El driver unificado mantiene:

- A21 en PH4.
- WE en PH3.
- A20 en PL4.
- Unlock `555/2AA`.
- Word Program x16.
- FastWrite S/X/Z.
- CRC y lectura de 8 MiB.

## Integración

Los algoritmos de programación se mantienen separados. El controlador superior solo selecciona un driver, aplica el pinout y exige ID exacto antes de delegar operaciones.
