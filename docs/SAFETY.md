# Seguridad de selección y hardware

Los tres chips comparten encapsulado SOP44, pero no las mismas funciones en los pines 1, 33 y 44. El firmware no puede auto-detectarlos sin emitir comandos de escritura de bus, por lo que la selección correcta depende del usuario.

La aplicación aplica cuatro barreras:

1. Selección explícita del modelo.
2. Casilla de confirmación física.
3. Clave de perfil distinta enviada al firmware.
4. Check ID exacto antes de cualquier operación.

Después de ID correcto el perfil queda bloqueado hasta reiniciar. Para cambiar de chip, desconecta, cambia físicamente el dispositivo, selecciona el nuevo perfil y vuelve a conectar.

No uses señales de 5 V directamente. Mantén 3.3 V estable, GND común y desacoplo cerca del chip.
