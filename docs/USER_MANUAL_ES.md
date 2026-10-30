# Manual vigente — MegaBurner MultiChip v0.7A + shield R4.9

No utilizar este manual con los cables/pinouts históricos v0.5A. Antes de alimentar, seguir [requisitos y lista de comprobación R4.9](PCB_R49_WIRING_AND_POWER.md).

## Preparación

- Equipo: Mega 2560 **ya modificado** (contacto POWER marcado `5V` mide 3,2–3,4 V), shield R4.9, adaptador SA638-B006 con continuidad y orientación certificadas, socket SOP44 y una flash admitida.
- Firmware: `firmware/MegaBurner_MultiChip_v0_7A_ProfileSafe/MegaBurner_MultiChip_v0_7A_ProfileSafe.ino` (con los `.cpp/.h` de la misma carpeta). Interfaz: `index.html`; requiere navegador con Web Serial (p.ej. Chrome/Edge en entorno compatible).
- Los archivos de las ramas `baselines/` son referencias de procedencia, **no** el firmware integrado.
- Baudrate de firmware/web: **500000**. No usar 1000000 como supuesto modo probado.

## Flujo operativo

1. Desconectar alimentación y USB antes de insertar o retirar el dispositivo. Insertar la flash respetando PIN1.
2. Abrir `index.html`, seleccionar referencia física exacta: MX29L3211, MX26L6420MC-90 o MX29LV160CTMC-90, y marcar confirmación.
3. Conectar el puerto; el navegador envía una clave `P1,3211`, `P2,6420` o `P3,160C` al firmware.
4. Ejecutar **Check ID**; verificar correspondencia exacta fabricante/dispositivo y `OK=1`. Si no coincide, detener operaciones y apagar antes de tocar el hardware.
5. Cargar la imagen ROM/flash del tamaño exacto: 4 MiB / 8 MiB / 2 MiB según perfil (revisar padding/preparación antes de grabar).
6. Guardar un **dump/backup** si existe contenido que deba conservarse. Después, **Erase** seguido de **Write**. Esto borra la memoria completa.
7. Verificar **CRC** y, para evidencia completa, **Read/Dump → Comparar** el binario. Si falla, revisar y repetir desde una secuencia de borrado controlada; no asumir programación correcta.
8. Finalizar la sesión y apagar antes de cambiar la memoria; al cambiar de perfil, cerrar la conexión y reiniciar el dispositivo para restablecer el bloqueo.

## Parámetros por memoria

| Memoria | Archivo | Página del protocolo | FastWrite | Estado pruebas |
|---|---:|---:|---|---|
| MX29L3211 | 4 MiB | 256 o 128 bytes (256 preferido) | Sí | Heredado v0.5A; R4.9 físico pendiente |
| MX26L6420MC-90 | 8 MiB | 2 bytes (palabra) | Sí | Heredado v0.1; R4.9 físico pendiente |
| MX29LV160CTMC-90 | 2 MiB | 2 bytes (palabra) | **No (SafeWrite)** | Sólo prueba lógica; físico pendiente |

Bloques de transferencia habituales de la herramienta: escritura 4096 bytes, lectura 16384 bytes. Estos son tamaños de transferencia serial, **no** páginas de programación para los perfiles word-program.

## Respuestas e incidencias

- `!N`: perfil no seleccionado; `!K`: clave equivocada; `!L`: cambio no permitido tras bloqueo; `!I`: secuencia/no ID/erase/sesión invalidados.
- `!F`: FastWrite no admitido; `!G`: tamaño de página incorrecto; `!A`, `!B` o `!O`: alineación, tamaño o dirección inválidos.
- Si el chip no responde: no ejecutar erase/write; consultar [diagnóstico R4.9](TROUBLESHOOTING_R49.md).
- `Q` en v0.7A informa de **estado de perfil**, no del registro de estado de v0.5A.

Para consultar comandos completos: [PROTOCOLO v0.7A](PROTOCOL.md).
