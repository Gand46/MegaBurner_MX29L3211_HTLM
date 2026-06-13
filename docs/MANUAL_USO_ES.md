# Manual de uso — MegaBurner Web Serial v0.5E MX29L3211

## Requisitos

- Arduino Mega 2560 modificado o adaptado para trabajar con señales de 3.3 V.
- Firmware de `firmware/current/` cargado en el Arduino.
- Navegador compatible con Web Serial: Chrome o Edge.
- MX29L3211 conectada conforme al pinout.
- Archivo ROM/imagen de hasta 4 MB.

## Flujo recomendado

1. Abrir `index.html`.
2. Conectar el Arduino por USB.
3. En opciones avanzadas, confirmar baudrate igual al `.ino` compilado. Valor final recomendado: 500000.
4. Pulsar **Conectar**.
5. Pulsar **Check ID**. El ID esperado es `C2F9`.
6. Seleccionar la ROM.
7. Preparar imagen.
8. Ejecutar **Erase**.
9. Ejecutar **Write**.
10. Ejecutar **CRC Verify rápido**.
11. Opcional: ejecutar **Read/Dump** y comparar dump contra imagen preparada.

## Skip FF

La opción “Saltar páginas 0xFF después de Erase OK” está desactivada inicialmente. Solo debe activarse manualmente cuando se haya hecho un Erase correcto en la misma sesión. Evita programar páginas que ya quedaron en `0xFF` tras el borrado.

## Solución de problemas

- Si `Check ID` no devuelve `C2F9`, no escribas la memoria.
- Si hay timeout, revisa baudios, puerto, cable USB y firmware.
- Si el CRC falla, haz dump completo y compara.
- Si aparece diferencia tipo `dump=0x10` y `preparada=0x30`, puede indicar falta de erase previo.
