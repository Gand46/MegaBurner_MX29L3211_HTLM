# Índice vigente — MegaBurner v0.7A + PCB R4.9-2L-FINAL

**Alcance:** firmware MultiChip ProfileSafe v0.7A, aplicación Web Serial, PCB R4.9-2L-FINAL de dos capas y documentación histórica de la rama MX29L3211. Identificador de integración: `v0.7A+PCB-R4.9-2L-FINAL-docs.2`. No se publica firmware v0.7B ni revisión de PCB R5.

## Orden de autoridad

1. **Hardware físico verificable + datasheet de cada chip**: seguridad del montaje/mediciones reales.
2. **Diseño vigente R4.9**: `hardware/pcb/R4.9-2L-FINAL/kicad/*.kicad_pcb`, `docs/NETLIST_AND_PINOUT.csv` y archivos de fabricación originales.
3. **Firmware v0.7A**: `firmware/MegaBurner_MultiChip_v0_7A_ProfileSafe/` y `config/chips.json` (comandos y modos del bus).
4. **Manuales actualizados** en esta carpeta.
5. **Archivo histórico v0.5A/v0.5E**: exclusivamente atribución, evolución y resultados históricos, nunca guía de cableado o protocolo en el hardware actual.

Ante discrepancias entre el CSV y el PCB, detener el montaje y resolverla antes de fabricar/conectar. Ni el CSV ni la documentación sustituyen la continuidad en el adaptador físico.

## Documentación por tarea

| Necesidad | Documento actual |
|---|---|
| Preparar físicamente la placa | [PCB R4.9: alimentación, pinout, comprobaciones](PCB_R49_WIRING_AND_POWER.md) |
| Consulta completa pin 1–44 | [Tabla CSV íntegra](../hardware/pcb/R4.9-2L-FINAL/docs/NETLIST_AND_PINOUT.csv) |
| Fuentes KiCad y Gerber | [README nativo PCB](../hardware/pcb/R4.9-2L-FINAL/README.md) y [fuente PCB-only](../hardware/pcb/R4.9-2L-FINAL/kicad/PCB_ONLY_SOURCE.md) |
| Primer montaje | [Checklist original R4.9](../hardware/pcb/R4.9-2L-FINAL/docs/ASSEMBLY_AND_FIRST_POWER.md) |
| Uso y diagnóstico Web Serial | [Manual de uso actualizado](USER_MANUAL_ES.md) · [Diagnóstico](TROUBLESHOOTING_R49.md) |
| Compatibilidad/selección | [Tabla por chip](CHIP_COMPATIBILITY.md) · [Pinout comparado](PINOUT_SOP44_COMPARISON.md) |
| Protocolo v0.7A | [Protocolo vigente](PROTOCOL.md) · [Migración v0.5A→v0.7A](PROTOCOLO_MIGRACION.md) |
| Créditos, licencias, fuentes | [Créditos y linaje](CREDITS_AND_LINEAGE.md) · [Licencia del proyecto](../LICENSE) |
| Rendimiento | [Resultados históricos y limitaciones](BENCHMARKS_HISTORICAL.md) |
| Validación y pendientes | [Auditoría de integración](INTEGRATION_VALIDATION.md) · [Pruebas v0.7A](TEST_REPORT.md) · [Auditorías originales R4.9](../hardware/pcb/R4.9-2L-FINAL/validation/VALIDATION_REPORT.md) |
| Qué pasó con los documentos anteriores | [Matriz de migración v0.5A](LEGACY_MIGRATION_MATRIX.md) y [archivo original](archive/MegaBurner_WebSerial_v0_5A/ARCHIVE_README.md) |

## Límite de alcance

El PCB recibido se denomina **PCB-only**: contiene `.kicad_pcb`, `.kicad_pro`, Gerbers, drill, BOM/CPL, SVG/PNG y auditorías, pero no `.kicad_sch`. **No afirmar ERC de esquemático**; los PASS provienen de auditorías declaradas por la entrega y de verificaciones reproducidas señaladas por separado. El diseño tiene 85 × 56 mm, dos capas, sin level-shifters ni fuente propia.

## README docs.2 / imágenes

- [README completo bilingüe, inglés primero y español después](../README.md).
- [Manifiesto con fuentes y hashes de la galería](GALLERY_MANIFEST.json).
- [Registro de validación docs.2](README_DOCS2_VALIDATION.md).
