MegaBurner WebSerial v0.4C FastCRC
===================================

Objetivo:
- Mantener v0.4A FastRead y v0.4B FastWrite ya validados.
- Agregar verificacion rapida CRC32 calculada directamente por Arduino leyendo la MX29L3211.

Cambios principales frente a v0.4B:
1. Nuevo comando Koffset,size\n.
   - Ejemplo: K0,4194304\n
   - Devuelve 8 caracteres ASCII hex con el CRC32 estandar.
2. El HTML calcula CRC32 local de la imagen preparada y lo compara contra el CRC32 calculado en la flash.
3. Evita enviar 4 MB de vuelta al navegador cuando solo se quiere verificar integridad.
4. Se conserva Read/Dump completo para validacion profunda y descarga del contenido.
5. Se conserva FastWrite S/X/Z sin cambios funcionales.

Orden de prueba recomendado:
1. Cargar firmware v0.4C.
2. Abrir HTML v0.4C.
3. Conectar a 500000 baudios.
4. Check ID.
5. Cargar ROM de 4 MB y preparar imagen modo none.
6. Erase.
7. Write en modo rapido v0.4B.
8. Pulsar CRC Verify rapido v0.4C.
9. Opcional: hacer Read/Dump completo y comparar dump vs preparada para confirmar.

Nota:
CRC Verify rapido confirma igualdad por CRC32. Para una validacion byte a byte exacta, usa Read/Dump + Comparar.
