# Diagnóstico — v0.7A + PCB R4.9

## El chip no responde a Check ID / `OK=0`

Detener operaciones destructivas. Apagar antes de inspeccionar. Comprobar 3,2–3,4 V real en J1.23, GND en J1.13/J1.32, el perfil seleccionado, orientación y DIP44→SOP44 con polímetro. Contrastar los tres pines variables (1/33/44) y el PCB R4.9. No diagnosticar únicamente por mensajes seriales: un perfil equivocado puede dirigir señales incorrectas aun antes del ID.

## El Mega se reinicia, la sesión cae o aparecen errores USB

Comprobar fuente 3,3 V, corriente bajo carga, relojería del ATmega2560, interfaz USB-serie, posibles retornos a 5 V, cableado y continuidad del adaptador. `500000` es el valor esperado por este firmware. El ZIP no acredita estabilidad del hardware R4.9 montado.

## Timeout, CRC distinto o datos de bus corruptos

Revisar contacto de las 16 líneas Q0–Q15 (especialmente Q12/J1.25/D26 según registro histórico del PCB), dirección A0–A21 por perfil, estado CE/OE/WE y verificación dump completa. Tras cambiar contenido en NOR flash, normalmente se requiere erase; no forzar escrituras sobre bits que necesitarían transición 0→1.

## Los archivos de v0.5A dejan de funcionar

La v0.7A exige selección del perfil y `Check ID` antes de leer/escribir; `Q` cambió de significado y `L` dejó de existir. Usar la interfaz `index.html` incluida en v0.7A, no la interfaz histórica. Consultar [migración de protocolo](PROTOCOLO_MIGRACION.md).

## El Gerber indica PASS pero el dispositivo falla

PASS de las auditorías entregadas significa chequeos geométricos/conectividad registrados; **no** montaje físico, prueba con socket SA638-B006, compatibilidad de alimentación ni grabación real con las tres referencias.
