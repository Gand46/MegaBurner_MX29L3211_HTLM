MegaBurner v0.4A FastRead
==========================

Objetivo:
- Acelerar lectura/dump/verify sin tocar la ruta de escritura validada.

Cambio aplicado:
- En MegaBurner_v0_4A_FastRead.ino, la función read(String param) ya NO ejecuta mx29l3211.reset() antes de cada bloque.
- El reset original incluye delay(500) dentro de MX29L3211::reset(). En un dump de 4 MB con 1024 bloques, eso agregaba ~512 segundos artificiales.

Sin cambios:
- Serial.begin(500000) se conserva.
- Check ID conserva reset después de leer ID.
- Erase conserva reset antes de borrar.
- Write conserva exactamente el flujo validado: handshake &, recepción de bloque, write16(), %, delay(100), reset().
- MX29L3211.cpp y MX29L3211.h se conservan sin cambios.

Prueba recomendada:
1. Compilar y cargar este sketch en Arduino Mega 2560.
2. Abrir megaburner_webserial_v0_4A_fastread.html.
3. Conectar a 500000 baudios.
4. Cargar la misma ROM de 4 MB y preparar imagen en modo none.
5. Ejecutar Check ID.
6. Ejecutar Read/Dump con readBlock=4096.
7. Comparar dump vs preparada.
8. Exportar TXT/CSV.
9. Si todo coincide, probar readBlock=8192 y luego 16384.

Criterio de éxito:
- Comparación OK / CRC32 igual.
- Read/Dump baja claramente frente a v0.3, que midió 11m31s y 675 ms/bloque.

