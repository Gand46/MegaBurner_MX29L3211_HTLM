# Manual de uso — MegaBurner Web Serial v0.5E MX29L3211

La aplicación principal del repositorio está en inglés, pero el flujo de uso recomendado es:

1. Abrir `web/index.html` en Chrome o Edge.
2. Conectar el Arduino Mega a 500000 baudios.
3. Verificar ID: debe devolver `C2F9`.
4. Cargar la ROM/imagen.
5. Preparar imagen.
6. Borrar chip con Erase.
7. Programar con Write.
8. Verificar con Fast CRC.
9. Si se requiere validación profunda, hacer Read/Dump y Compare.

Valores finales recomendados: 500000 baudios, Fast Write, writeBlock 4096, pageSize 256, readBlock 16384.
