# Protocolo serial MultiChip v0.7A

Todos los comandos terminan en `\n`. Baudrate: 500000.

| Comando | Función |
|---|---|
| `V` | Versión y lista de perfiles |
| `P1,3211` | Seleccionar MX29L3211 |
| `P2,6420` | Seleccionar MX26L6420 |
| `P3,160C` | Seleccionar MX29LV160CTMC |
| `Q` | Estado del perfil y latches |
| `C` | Leer ID; bloquea el perfil si coincide |
| `E` | Erase: `&` al iniciar, `%` al terminar |
| `S` | Iniciar sesión de escritura |
| `Woffset,page,size` | Escribir bloque Safe |
| `Xoffset,page,size` | Escribir bloque Fast, solo perfiles validados |
| `Z` | Finalizar sesión y reset |
| `Roffset,size` | `&` y luego bytes crudos |
| `Koffset,size` | CRC32 en 8 caracteres hex |

## Interlocks

- Sin perfil: `!N`.
- Clave de perfil errónea: `!K`.
- Perfil bloqueado: `!L`.
- ID/erase/sesión no válidos: `!I`.
- Fast no soportado: `!F`.
- Page-size incompatible: `!G`.
- Alineación/rango: `!A`, `!B`, `!O`.

## Migración desde v0.5A

En v0.7A los comandos `P1,3211`, `P2,6420` o `P3,160C` deben preceder a `C`. El antiguo `Q` de estado del chip se sustituyó por información del **perfil/cerrojos** y `L` antiguo (Clear Status) no forma parte del protocolo actual. El operador no debe usar scripts seriales de v0.5A contra esta versión. Ver [`PROTOCOLO_MIGRACION.md`](PROTOCOLO_MIGRACION.md).
