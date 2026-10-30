# Historial de desarrollo

## Origen

MegaBurner original usa Arduino Mega 2560, firmware C++ y una aplicación Java/SWT para PC. Este port conserva la función de hardware y reemplaza la aplicación de PC por HTML/Web Serial.

## Etapas

- v0.1: primera interfaz HTML/Web Serial.
- v0.2: corrección de ACK `&`/`%`; escritura funcional.
- v0.3: diagnóstico con métricas y CSV.
- v0.4A: FastRead; se eliminó reset por bloque de lectura.
- v0.4B: FastWrite S/X/Z; se eliminó reset por bloque de escritura.
- v0.4C: FastCRC calculado en Arduino.
- v0.5A: PageSize 256, tamaño nativo de página para MX29L3211.
- v0.5B: prueba 1.000.000 baudios; descartada por inestabilidad real.
- v0.5E: versión final 500000 baudios, SafeCore, CRC optimizado, UI XP, documentación GitHub.

## Resultado

La aplicación Java/DLL fue sustituida por un único HTML portable. El firmware fue optimizado sin abandonar Arduino Mega 2560.
