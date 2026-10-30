# Benchmarks

Resultados observados durante el desarrollo del port HTML/Web Serial.

| Versión | Operación | Parámetros | Tiempo aprox. | Resultado |
|---|---|---|---:|---|
| v0.3 | Write | 500000, block 4096, page 128 | 17m44s | OK |
| v0.4A | Read/Dump | 500000, readBlock 16384 | 1m49s | OK |
| v0.4B | Write | 500000, block 4096, page 128 | 3m04s–3m51s | OK |
| v0.4C | CRC Verify | 500000 | 1m04s | OK |
| v0.5A | Write | 500000, block 4096, page 256 | Validado con 2 ROMs | OK |
| v0.5B | 1000000 baud | prueba real | Inestable | Descartado |

La configuración final se mantiene en 500000 baudios por estabilidad.
