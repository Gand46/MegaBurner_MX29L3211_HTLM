# Changelog

## v0.7A ProfileSafe

- Integración de MX29L3211 v0.5A y MX26L6420 Socket v0.1 sin mezclar algoritmos de programación.
- Nuevo perfil experimental MX29LV160CTMC-90.
- Boot sin perfil y pines universales seguros.
- Selección con claves de confirmación.
- ID exacto obligatorio y perfil bloqueado tras validación.
- Erase obligatorio antes de una sesión de escritura.
- UI dinámica que elimina opciones incompatibles.
- SafeWrite obligatorio para MX29LV160CT.
- Lectura por offset con ACK previo.
- Comparación de dump con conteo total de diferencias.
- Pruebas de C++, JavaScript, perfiles, rangos y mapeo de puertos.

## Integración documental docs.1 (firmware v0.7A + PCB R4.9-2L-FINAL)

- Se incorporó la placa KiCad y archivos Gerber/BOM/CPL/fotos/validaciones sin modificar los originales.
- Se preservó el ZIP documental v0.5A bajo archivo histórico, con matriz de sustituciones y advertencias de pinout/potencia/protocolo obsoletos.
- Se creó índice de documentación, manual integrado, pinout de 44 pines desde CSV, diagnóstico, protocolo de migración, créditos y benchmarks históricos.
- Se añadió test estático de integración PCB/CSV/perfiles/firmware y se hizo portable el lanzador de pruebas con `bash`.
- No se cambió el firmware ni el HTML de la v0.7A y no se alteró la PCB R4.9.
- No se declara montaje, programación real, compatibilidad eléctrica demostrada ni compilar Arduino AVR en este entorno.

## Documentation release docs.2 — bilingual README and authentic gallery (2026-10-30)

- Rewrote the root README in English and Spanish, in the requested nine-section order (including 6.5 PCB).
- Integrated 12 optimized assets from the supplied prototype/PCB image archive, with hash provenance.
- Included full J1 44-contact table, per-chip procedures, Web Serial/firmware commands, safety gates, R4.9 build checklist, and AI disclosure.
- Kept v0.7A firmware, HTML, config, tests, baselines, and R4.9 KiCad/Gerber assets untouched.
- A distinct verification record is supplied for documentation release; no physical R4.9 certification is claimed.
