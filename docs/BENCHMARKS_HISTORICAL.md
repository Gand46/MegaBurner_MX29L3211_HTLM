# Benchmarks históricos — procedencia de v0.5A

El documento original [`docs/BENCHMARKS.md`](archive/MegaBurner_WebSerial_v0_5A/docs/BENCHMARKS.md) conserva medidas reportadas durante el desarrollo MX29L3211. No se reprodujeron en el shield R4.9 ni deben extrapolarse a chips de 8 o 2 MiB.

| Origen | Acción | Registro original | Categoría |
|---|---|---|---|
| v0.3 | Escritura MX29L3211 (page128) | ~17m44s | Histórico, sin trazas incluidas |
| v0.4A | Dump MX29L3211 | ~1m49s | Histórico, sin trazas incluidas |
| v0.4B | Escritura MX29L3211 (page128) | ~3m04s–3m51s | Histórico, sin trazas incluidas |
| v0.4C | CRC MX29L3211 | ~1m04s | Histórico, sin trazas incluidas |
| v0.5A | Escritura page256 | 2 ROMs, sin tiempo exacto | Histórico |
| v0.5B | 1000000 baudios | Inestable | Descartado históricamente |

**Vigente:** comunicación v0.7A = 500000 baudios. Las pruebas de programación física con la nueva PCB R4.9 se consideran pendientes, sin inventar un benchmark nuevo.
