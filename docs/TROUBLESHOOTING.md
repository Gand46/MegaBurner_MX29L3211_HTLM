# Solución de problemas

## Check ID no devuelve C2F9

- Verifica baudios.
- Verifica cableado.
- Verifica alimentación de 3.3 V.
- Verifica `BYTE` en HIGH.
- No ejecutes Erase/Write hasta resolverlo.

## Timeout

- El HTML y el `.ino` deben usar los mismos baudios.
- Usa 500000 como perfil final recomendado.
- Revisa cable USB y puerto.
- Reconecta y limpia RX.

## CRC falla

- Realiza Read/Dump completo.
- Compara contra imagen preparada.
- Verifica que hiciste Erase antes de escribir una ROM distinta.

## Diferencias tipo dump=0x10 preparada=0x30

Puede indicar que se intentó escribir sin borrar previamente. En NOR flash no se pueden cambiar bits de 0 a 1 sin erase.

## 1.000.000 baudios

Fue probado y resultó inestable en el entorno de desarrollo. No se recomienda como perfil final.
