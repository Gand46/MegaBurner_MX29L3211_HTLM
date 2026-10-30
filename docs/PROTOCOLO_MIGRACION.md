# Migración de protocolo Web Serial v0.5A → MultiChip v0.7A

El protocolo antiguo sólo consideraba MX29L3211. La versión v0.7A emplea 500000 baudios y comandos ASCII terminados en `\n`, con datos binarios de lectura/escritura según operación. No ejecutar herramientas seriales anteriores de forma automática contra un shield nuevo.

| Acción | Documentación anterior | v0.7A vigente |
|---|---|---|
| Seleccionar memoria | Implícita (único chip) | Obligatorio `P1,3211`, `P2,6420`, `P3,160C` |
| Consultar chip | `C` | `C` después de perfil; verifica ID exacto y bloquea perfil |
| Consultar estado | `Q` = registro de estado de flash | `Q` = perfil, tamaño, IDOK, ERASEOK, LOCKED |
| Borrar | `E` | `E` sólo con ID válido |
| Escribir | `W...`, `S` + `X...` + `Z` | Mismos códigos, pero requieren perfil, ID, erase y sesión |
| Page-size | 128/256 para MX29L3211 | 128/256 para 3211, **2** para 6420 y 160CT |
| FastWrite | Para MX29L3211 | 3211 y 6420; **bloqueado en 160CT** |
| Leer/CRC | `R`, `K` | `R`, `K` sólo con ID validado |
| Limpiar registro status | `L` (heredado) | No está implementado |

Códigos y campos actuales: [docs/PROTOCOL.md](PROTOCOL.md), firmware `.ino` e interfaz `index.html`. El firmware v0.7A mantiene bytes de reconocimiento `&`/`%` y error `!`. **No se debe interpretar el Q histórico como Q actual.**
