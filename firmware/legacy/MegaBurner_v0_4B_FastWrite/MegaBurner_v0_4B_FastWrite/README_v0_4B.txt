MegaBurner WebSerial v0.4B FastWrite
=====================================

Objetivo:
- Mantener la lectura rápida validada en v0.4A.
- Agregar escritura rápida de bajo riesgo relativo sin eliminar el modo seguro.

Cambios principales:
1. Comandos ASCII terminados en \n para evitar String + delay(10) por byte.
2. ACK binario con Serial.write('&'), Serial.write('%') y Serial.write('!').
3. W = modo seguro: escribe bloque y ejecuta reset() después de cada bloque.
4. S/X/Z = modo rápido v0.4B:
   - S: begin fast write, reset inicial una vez.
   - Xoffset,page,size: escribe bloque sin reset al final.
   - Z: finish fast write, reset final una vez.
5. MAX_WRITE_BLOCK fijo en 4096 para proteger SRAM del Arduino Mega.

Orden de prueba recomendado:
1. Cargar firmware v0.4B.
2. Abrir HTML v0.4B.
3. Conectar a 500000 baudios.
4. Check ID.
5. Erase.
6. Write en modo seguro una vez si se quiere validar compatibilidad.
7. Write en modo rápido v0.4B con writeBlock=4096 y pageSize=128.
8. Read/Dump con readBlock=16384.
9. Comparar dump vs imagen preparada.

Nota:
El modo rápido elimina reset()+delay(500) por cada bloque de 4096 bytes.
Si falla verify, volver al modo seguro W y reportar offset.
