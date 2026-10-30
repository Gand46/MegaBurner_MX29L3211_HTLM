# Manual de uso

1. Conecta el chip con el sistema apagado.
2. Abre `index.html`.
3. Selecciona la referencia exacta y marca la confirmación.
4. Pulsa **Conectar y aplicar perfil**.
5. Pulsa **Check ID**. No continúes si `OK=0`.
6. Carga el archivo y prepara la imagen al tamaño exacto.
7. Pulsa **Erase**.
8. Pulsa **Write**.
9. Ejecuta **CRC rápido**.
10. Ejecuta **Read/Dump** y **Comparar**.

## Reglas por chip

- MX29L3211: Page 256 recomendado; Fast validado.
- MX26L6420: Page 2/Word; Fast validado.
- MX29LV160CT: Page 2/Word; Safe obligatorio hasta validación.

No cambies el selector mientras existe conexión. Para otro chip, desconecta y reinicia el flujo.
