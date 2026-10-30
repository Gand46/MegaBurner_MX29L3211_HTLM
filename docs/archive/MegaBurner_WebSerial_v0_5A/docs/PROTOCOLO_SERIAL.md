# Protocolo serial

El firmware utiliza comandos ASCII sobre USB serial. El baudrate debe coincidir entre el `.ino` compilado y el navegador.

## Comandos

| Comando | Uso |
|---|---|
| `C` | Check ID. Esperado para MX29L3211: `C2F9`. |
| `E` | Chip erase. |
| `R<block>,<size>` | Leer bloque. |
| `W<offset>,<page>,<size>` | Escritura segura. |
| `S` | Iniciar FastWrite. |
| `X<offset>,<page>,<size>` | Escribir bloque en FastWrite. |
| `Z` | Finalizar FastWrite. |
| `K<offset>,<size>` | CRC32 calculado en Arduino. |
| `V` | Versión/capacidades. |
| `Q` | Status register. |
| `L` | Clear status register. |

## Respuestas

| Respuesta | Significado |
|---|---|
| `%` | Operación OK. |
| `&` | Arduino listo para recibir bloque. |
| `!P` | Program fail. |
| `!E` | Erase fail. |
| `!T` | Timeout. |
| `!R` | Timeout recibiendo datos. |
| `!C` | Comando inválido. |
| `!B` | Tamaño de bloque inválido. |
| `!A` | Alineación inválida. |
