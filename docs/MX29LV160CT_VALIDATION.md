# Validación del perfil MX29LV160CTMC-90

## Confirmado por datasheet

- MX29LV160C es 16 Mbit, 2M × 8 o 1M × 16.
- Alimentación de lectura/programación/borrado: 2.7–3.6 V.
- CT corresponde a Top Boot.
- En SOP44: pin 1 RESET#, pin 33 BYTE#, pin 44 WE#.
- ID word-mode: fabricante `00C2`, dispositivo Top Boot `22C4`.
- Word Program utiliza unlock `555/2AA` y comando `A0`.
- Chip Erase utiliza la secuencia AMD/JEDEC habitual.
- DQ7/DQ6/DQ5 permiten seguimiento de programación/borrado.

## Implementación en v0.7A

- RESET# en D7/PH4, siempre HIGH durante operaciones normales.
- BYTE# en D6/PH3, HIGH para x16.
- WE# en D45/PL4.
- A16–A19 en PL0–PL3; `setAddress()` preserva PL4.
- Word Program, page-size 2.
- SafeWrite obligatorio.
- Verificación completa de cada bloque y hasta dos reintentos de palabras corregibles 1→0.
- Blank-sample después de erase.

## Pendiente

- Compilación en Arduino IDE/CLI real en el equipo del usuario.
- Check ID con chip físico.
- Erase, write de 2 MiB, CRC y dos dumps consecutivos.
- Confirmación de sectores no protegidos.

Hasta completar estas pruebas, el perfil debe considerarse experimental.
