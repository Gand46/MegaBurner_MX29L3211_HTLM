MegaBurner Web Serial v0.5B - Baud1M Turbo
================================================

Base: v0.5A NativePage256 validada sobre MX29L3211.

Cambios frente a v0.5A:
- SERIAL_BAUD cambia de 500000 a 1000000.
- pageSize 256 se mantiene como perfil recomendado.
- writeBlock 4096 se mantiene.
- readBlock recomendado inicial: 16384 para aislar la variable baudrate.
- readBlock experimental: 32768 si 16384 pasa CRC/dump.
- FastRead, FastWrite S/X/Z y FastCRC K se conservan.

IMPORTANTE:
El firmware y el HTML deben usar el mismo baudrate. Si cargas este firmware
v0.5B, abre el HTML a 1000000 baudios. Si vuelves al firmware v0.5A, usa
500000 baudios.

Flujo de prueba recomendado:
1. Cargar firmware v0.5B en Arduino Mega 2560.
2. Abrir megaburner_webserial_v0_5B_baud1m.html.
3. Conectar a 1000000 baudios.
4. Check ID: debe devolver C2F9.
5. Cargar ROM de 4 MB sin espejo.
6. Preparar imagen modo none.
7. Erase.
8. Write fast, writeBlock 4096, pageSize 256.
9. CRC Verify rápido.
10. Read/Dump con readBlock 16384 y comparar.
11. Si todo OK, probar readBlock 32768.

Valores recomendados:
- baudrate: 1000000
- writeMode: fast
- writeBlock: 4096
- pageSize: 256
- readBlock inicial: 16384
- readBlock experimental: 32768
- verify principal: CRC Verify rápido
- verify profundo: Read/Dump + Comparar
