# Informe de pruebas v0.7A

## Fuentes base

- MX29L3211: firmware y HTML exactos de `MegaBurner_v0_5A_NativePage256`.
- MX26L6420: firmware y HTML exactos de `MegaBurner Web Serial MX26L6420 Socket-Compatible v0.1`.
- MX29LV160CT: datasheet Macronix MX29LV160C T/B.

## Pruebas ejecutadas

### Compilación C++ host

Todos los `.cpp` y el `.ino` se compilaron con un stub Arduino usando:

```text
g++ -std=c++17 -Wall -Wextra -Wconversion -Wshadow
```

Resultado: `Host C++ compile OK`, sin errores.

### JavaScript

El script del HTML fue extraído y validado con:

```text
node --check tests/ui.js
```

Resultado: sin errores de sintaxis.

### Lógica de perfiles

`tests/test_logic.py` comprobó:

- IDs y tamaños exactos de los tres perfiles.
- Claves `3211`, `6420` y `160C`.
- Interlocks de perfil, ID, erase y sesión.
- FastWrite solo para los dos perfiles validados.
- Page-size 256/128 únicamente para MX29L3211.
- Page-size 2 para MX26L6420 y MX29LV160CT.
- A21 cambia en la frontera de 4 MiB del MX26L6420.
- PL4/WE permanece HIGH al cambiar A16–A19 en MX29LV160CT.
- Mapeos PH4/PH3/PL4 por perfil.

Resultado: `All multichip logic tests passed`.

## Cobertura de hardware

| Perfil | Compilación/lógica | Hardware real |
|---|---:|---:|
| MX29L3211 | Sí | Sí, por las pruebas heredadas v0.5A |
| MX26L6420 | Sí | Sí, dos chips con write/dump correcto |
| MX29LV160CT | Sí | Pendiente |

## Limitación honesta

No es posible garantizar ausencia total de errores de hardware sin probar cada perfil en el chip físico, fuente, socket y placa concretos. La rama MX29LV160CT se entrega bloqueada en modo Safe y marcada experimental hasta completar Check ID, erase, write, CRC y doble dump.

## Relación con PCB R4.9

Esta documentación de pruebas v0.7A es previa a la integración de los archivos KiCad. Se añade una validación **estática** de las 44 asignaciones declaradas en KiCad/CSV, los tres perfiles y las rutas de alimentación en [`INTEGRATION_VALIDATION.md`](INTEGRATION_VALIDATION.md). No se realizó prueba de encendido, montaje ni grabación física sobre una R4.9 terminada.
