# Validación de integración documental y estática — docs.1

## Evidencia disponible

- ZIP v0.7A: CRC ZIP correcto; checksums originales `SHA256SUMS.txt` verificables.
- ZIP R4.9: CRC ZIP correcto; checksums originales verificados.
- Proyecto KiCad PCB-only: archivo editable incluido; Gerbers, Excellon, BOM, CPL, previews y auditorías originales preservadas.
- Comparación estática a ejecutar desde la raíz: `python3 tests/test_pcb_r49_integration.py`. Comprueba 44 filas CSV, 44 pads J1 y sus nets KiCad, alimentación J1.13/23/32, los 3 pines variables, GPIO compartido con firmware, tamaño/clave/ID de perfiles, integridad de archivos originales del PCB.
- Suite firmware v0.7A: `bash tests/run_all.sh`. Incluye compilación host C++ con stub, pruebas lógica/estados/html/memoria, sintaxis JS y nuevo test estático de integración.
- **Sin cambios** en `.ino`, `.cpp`, `.h`, `index.html`, `web/index.html`, `config/chips.json` ni diseño KiCad/CAM.

## Herencia de evidencias, no nueva calificación de hardware

Las auditorías originales R4.9 **declaran** PASS de topología/DRC/CAM y entrega fabricable; la integración documental no certifica KiCad DRC independiente desde su ejecutable. Los perfiles 3211 y 6420 declaran pruebas anteriores con hardware; la placa nueva R4.9 **no** ha sido probada físicamente aquí. El perfil 160CT no está físicamente validado.

## Pendiente para liberación funcional con placa concreta

1. Medir tensiones: `POWER/5V` del Mega modificado a 3,2–3,4 V, J1.23 a 3,3 V e IOREF/GPIO adecuados; vigilar USB-serie y retroalimentación.
2. Medir continuidad DIP44↔SOP44 real, sin cortos, y alimentación bajo carga.
3. Inspeccionar el ensamble, soporte de clamp y señalización PIN1.
4. Compilar para la placa real con `arduino-cli` o Arduino IDE si no está instalado en el entorno, y después realizar `Check ID`, pruebas de lectura y programación con chips de prueba de los perfiles aplicables.
5. Confirmar funcionamiento del Mega 16 MHz al voltaje modificado y alimentación/desacoplo de la flash en el conjunto.

Estas limitaciones impiden declarar una validación física completa aunque las pruebas automatizadas resulten PASS.
