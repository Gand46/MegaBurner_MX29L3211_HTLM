# Matriz de compatibilidad

## MX29L3211

- Capacidad: 32 Mbit / 4 MiB.
- ID esperado: `00C2/00F9`.
- Pin 1: WE#.
- Pin 33: BYTE#; HIGH para x16.
- Pin 44: A20.
- Programación: Page Program.
- Page-size permitidos por esta herramienta: 256 y 128 bytes.
- FastWrite: habilitado y validado.

La ruta conserva la lógica de `MegaBurner_v0_5A_NativePage256`, incluyendo el tamaño de página 256, lectura por bloques de 16 KiB y CRC.

## MX26L6420MC-90

- Capacidad: 64 Mbit / 8 MiB.
- ID esperado: `00C2/22FC`.
- Pin 1: A21.
- Pin 33: WE#.
- Pin 44: A20.
- Programación: Word Program x16.
- Page-size de protocolo: 2 bytes, una palabra.
- FastWrite: habilitado y validado con dos chips físicos.

La ruta conserva el algoritmo v0.1 validado. La verificación final mediante dump completo continúa siendo obligatoria.

## MX29LV160CTMC-90

- Capacidad: 16 Mbit / 2 MiB, 1M × 16 en modo word.
- Variante: Top Boot (`CT`).
- ID esperado: fabricante `00C2`, dispositivo `22C4`.
- Pin 1: RESET#.
- Pin 33: BYTE#; HIGH para x16.
- Pin 44: WE#.
- Dirección máxima: A19; no usa A20/A21.
- Programación: Word Program mediante `AA@555`, `55@2AA`, `A0@555`, datos/dirección.
- FastWrite: bloqueado hasta validación física.

El driver mantiene RESET# y BYTE# altos, preserva PL4 como WE al actualizar A16–A19 y verifica cada bloque después de programarlo. Las palabras corregibles se reintentan hasta dos veces.
