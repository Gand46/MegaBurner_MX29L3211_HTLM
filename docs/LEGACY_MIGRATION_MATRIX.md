# Matriz de incorporación de documentación v0.5A a v0.7A + PCB R4.9

Todos los archivos útiles del ZIP documental original están archivados sin alteraciones bajo `docs/archive/MegaBurner_WebSerial_v0_5A/`. Los archivos auxiliares macOS `.DS_Store` y `__MACOSX` no forman parte de la documentación de referencia. Ningún dato obsoleto se presenta como cableado vigente.

| Documento heredado | Tratamiento actual |
|---|---|
| [`README.md`](archive/MegaBurner_WebSerial_v0_5A/README.md) | Índice histórico general: reemplazado por README principal v0.7A + DOCUMENTATION_INDEX; pinout/potencia/roadmap obsoletos. |
| [`MegaBurner_WebSerial_README_ES_EN.md`](archive/MegaBurner_WebSerial_v0_5A/MegaBurner_WebSerial_README_ES_EN.md) | Introducción y origen histórico; vigente README principal + CREDITS_AND_LINEAGE. |
| [`MegaBurner_Modificacion_Mega2560_3V3.md`](archive/MegaBurner_WebSerial_v0_5A/MegaBurner_Modificacion_Mega2560_3V3.md) | No sirve como guía directa R4.9; vigente PCB_R49_WIRING_AND_POWER (sin alternativas genéricas). |
| [`CHANGELOG.md`](archive/MegaBurner_WebSerial_v0_5A/CHANGELOG.md) | Historia v0.5A/v0.5E; vigente CHANGELOG raíz. |
| [`CONTRIBUTING.md`](archive/MegaBurner_WebSerial_v0_5A/CONTRIBUTING.md) | Antecedente general; documentación actual en DOCUMENTATION_INDEX. |
| [`docs/PINOUT_MX29L3211_MEGA2560.md`](archive/MegaBurner_WebSerial_v0_5A/docs/PINOUT_MX29L3211_MEGA2560.md) | OBSOLETO PELIGROSO: 13/23/38, datos y direcciones errados; reemplazado por CSV R4.9 + pinout vigente. |
| [`docs/SERIAL_PROTOCOL.md`](archive/MegaBurner_WebSerial_v0_5A/docs/SERIAL_PROTOCOL.md) | OBSOLETO: Q/L y falta P; reemplazado por PROTOCOL + PROTOCOLO_MIGRACION. |
| [`docs/PROTOCOLO_SERIAL.md`](archive/MegaBurner_WebSerial_v0_5A/docs/PROTOCOLO_SERIAL.md) | OBSOLETO: protocolo único chip; reemplazado por PROTOCOL + PROTOCOLO_MIGRACION. |
| [`docs/MODIFICACION_MEGA2560_3V3.md`](archive/MegaBurner_WebSerial_v0_5A/docs/MODIFICACION_MEGA2560_3V3.md) | Referencia histórica genérica; sustituido por PCB_R49_WIRING_AND_POWER. |
| [`docs/TROUBLESHOOTING.md`](archive/MegaBurner_WebSerial_v0_5A/docs/TROUBLESHOOTING.md) | Migrado a TROUBLESHOOTING_R49. |
| [`docs/BENCHMARKS.md`](archive/MegaBurner_WebSerial_v0_5A/docs/BENCHMARKS.md) | Conservado como antecedentes en BENCHMARKS_HISTORICAL. |
| [`docs/GITHUB_PUBLISH_CHECKLIST.md`](archive/MegaBurner_WebSerial_v0_5A/docs/GITHUB_PUBLISH_CHECKLIST.md) | Checklist de publicación v0.5A obsoleto: firmware/current no existe; vigente pruebas integración. |
| [`docs/CREDITS.md`](archive/MegaBurner_WebSerial_v0_5A/docs/CREDITS.md) | Créditos conservados y ampliados en CREDITS_AND_LINEAGE. |
| [`docs/CREDITOS.md`](archive/MegaBurner_WebSerial_v0_5A/docs/CREDITOS.md) | Créditos conservados y ampliados en CREDITS_AND_LINEAGE. |
| [`docs/USER_MANUAL.md`](archive/MegaBurner_WebSerial_v0_5A/docs/USER_MANUAL.md) | Manual único chip reemplazado por USER_MANUAL_ES v0.7A. |
| [`docs/MANUAL_USO_ES.md`](archive/MegaBurner_WebSerial_v0_5A/docs/MANUAL_USO_ES.md) | Manual único chip reemplazado por USER_MANUAL_ES v0.7A. |
| [`docs/ROADMAP.md`](archive/MegaBurner_WebSerial_v0_5A/docs/ROADMAP.md) | Hitos 64 Mbit/PCB ya materializados; no tratar pendientes históricos como plan vigente. |
| [`docs/HISTORIAL_DESARROLLO.md`](archive/MegaBurner_WebSerial_v0_5A/docs/HISTORIAL_DESARROLLO.md) | Conservado como historial; no sustituto del estado actual. |
| [`docs/DEVELOPMENT_HISTORY.md`](archive/MegaBurner_WebSerial_v0_5A/docs/DEVELOPMENT_HISTORY.md) | Conservado como historial; no sustituto del estado actual. |
| [`docs/assets/original-megaburner/megaburner_hw.png`](archive/MegaBurner_WebSerial_v0_5A/docs/assets/original-megaburner/megaburner_hw.png) | Referencia gráfica histórica, no representa R4.9. |
| [`docs/assets/original-megaburner/megaburner_window.png`](archive/MegaBurner_WebSerial_v0_5A/docs/assets/original-megaburner/megaburner_window.png) | Referencia gráfica histórica, no representa la UI v0.7A. |

## Correcciones de mayor impacto

- El antiguo `docs/PINOUT_MX29L3211_MEGA2560.md` asigna **VCC a SOP44.13/38 y GND a SOP44.23**, además de intercambiar Q0–Q15 y A12–A15. **No reutilizarlo**; la referencia actual corresponde a J1.13=GND, J1.23=VCC, J1.32=GND, J1.38=A12 y Q0 en 15/Q7 en 30.
- El antiguo README proponía D44/PL5 para A21 como conjetura futura; con el PCB R4.9 el perfil MX26L6420 emplea **SOP44.1→D7/PH4** como A21.
- El antiguo README describe opciones generales de alimentación a 3,3 V; el diseño R4.9 **exige una modificación específica** de la alimentación del Mega: el contacto llamado 5V debe estar en 3,3 V. El rail 3V3 no alimenta el shield.
- Los documentos de versión v0.5E/README v0.5A son históricos; el firmware actual sigue siendo v0.7A y el PCB sigue siendo R4.9-2L-FINAL.

Todas las correcciones de estas relaciones son documentales. No se modificaron algoritmos de programación ni pistas del PCB.
