# Seguridad de selección y hardware

Los tres chips comparten encapsulado SOP44, pero no las mismas funciones en los pines 1, 33 y 44. El firmware no puede auto-detectarlos sin emitir comandos de escritura de bus, por lo que la selección correcta depende del usuario.

La aplicación aplica cuatro barreras:

1. Selección explícita del modelo.
2. Casilla de confirmación física.
3. Clave de perfil distinta enviada al firmware.
4. Check ID exacto antes de cualquier operación.

Después de ID correcto el perfil queda bloqueado hasta reiniciar. Para cambiar de chip, desconecta, cambia físicamente el dispositivo, selecciona el nuevo perfil y vuelve a conectar.

No uses señales de 5 V directamente. Mantén 3.3 V estable, GND común y desacoplo cerca del chip.

## Condición específica del PCB R4.9-2L-FINAL

- J1.13 y J1.32 = GND. J1.23 = VCC nominal 3,3 V, conectado exclusivamente al contacto Mega POWER rotulado físicamente **5V**, que **debe estar modificado a 3,3 V**.
- El pin Mega POWER rotulado `3V3` está **sin conectar** en el shield R4.9; tampoco proporciona una alimentación de reserva.
- La placa no tiene regulador, conversores de nivel, aislamiento de 5 V, condensadores C1/C2 ni conector auxiliar. No basta añadir un suministro 3,3 V en el contacto marcado 3V3.
- La modificación concreta del Mega y la vía USB/alimentación deben comprobarse eléctricamente antes de conectar el shield. Incluye niveles de IOREF, GPIO y transceptor USB-serie, frecuencia/estabilidad del microcontrolador y desacoplo efectivo.
- El interlock por perfil/JEDEC ID **no protege** contra un Mega que aplique 5 V ni contra daños durante arranque/reset o chip mal insertado.
- Mantener apagado al insertar/retirar la flash. Comprobar que el adaptador DIP44→SOP44 conserva correspondencia pin a pin, sin asumirla de la fotografía.

[Pruebas antes de alimentar e insertar una flash](PCB_R49_WIRING_AND_POWER.md).
