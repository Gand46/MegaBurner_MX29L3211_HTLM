# MegaBurner Web Serial MultiChip v0.7A ProfileSafe

Firmware único y aplicación HTML/Web Serial para seleccionar y trabajar con tres memorias flash paralelas SOP44 mediante Arduino Mega 2560:

- **MX29L3211** — 4 MiB — ruta derivada de `MegaBurner_v0_5A_NativePage256`.
- **MX26L6420MC-90** — 8 MiB — ruta derivada de `MegaBurner Web Serial MX26L6420 Socket-Compatible v0.1`.
- **MX29LV160CTMC-90** — 2 MiB — perfil nuevo basado en datasheet, todavía pendiente de validación física.

> Esta versión no intenta auto-detectar el chip. El usuario debe seleccionar y confirmar físicamente la referencia antes de conectar. Los pines 1, 33 y 44 cambian de función entre los tres dispositivos; seleccionar un perfil equivocado puede aplicar señales incorrectas.

## Estado de soporte

| Chip | Tamaño | Algoritmo | FastWrite | Estado |
|---|---:|---|---:|---|
| MX29L3211 | 4 MiB | Page Program 256/128 bytes | Sí | Hardware validado |
| MX26L6420MC-90 | 8 MiB | Word Program x16 | Sí | Hardware validado con dos chips |
| MX29LV160CTMC-90 | 2 MiB | Word Program x16 + verificación de bloque | No | Validación lógica/datasheet; pendiente de chip físico |

## Seguridad por perfil

El firmware inicia sin perfil y mantiene CE/OE/WE potenciales en estado seguro. La aplicación envía una selección con clave específica:

```text
P1,3211
P2,6420
P3,160C
```

Después exige un `Check ID` exacto. Erase, Write, Read y CRC permanecen bloqueados hasta que fabricante y dispositivo coincidan. Una vez validado el ID, el perfil queda bloqueado hasta reiniciar/reconectar.

## Pinout crítico del socket SOP44

| Pin SOP44 | MX29L3211 | MX26L6420 | MX29LV160CT | Línea Arduino |
|---:|---|---|---|---|
| 1 | WE# | A21 | RESET# | D7 / PH4 |
| 33 | BYTE# | WE# | BYTE# | D6 / PH3 |
| 44 | A20 | A20 | WE# | D45 / PL4 |

El resto del bus principal conserva la distribución MegaBurner: A0–A7 en PORTF, A8–A15 en PORTK, Q0–Q7 en PORTC, Q8–Q15 en PORTA, CE en PH6 y OE en PH1.

## Inicio rápido

1. Carga `firmware/MegaBurner_MultiChip_v0_7A_ProfileSafe/MegaBurner_MultiChip_v0_7A_ProfileSafe.ino`.
2. Abre `index.html` con Chrome o Edge.
3. Selecciona el chip físico y marca la confirmación.
4. Conecta a 500000 baudios; la aplicación aplica el perfil.
5. Ejecuta **Check ID**.
6. Prepara una imagen con el tamaño exacto del chip.
7. Ejecuta **Erase → Write → CRC → Read/Dump → Comparar**.

## Validaciones ejecutadas en esta entrega

- Compilación C++ completa con stub de Arduino y advertencias estrictas.
- Sintaxis JavaScript validada con `node --check`.
- Pruebas de mapeo A20/A21 y preservación del bit WE de MX29LV160.
- Pruebas de tamaños, IDs, page-size y disponibilidad de FastWrite por perfil.
- Pruebas de interlocks: perfil, ID, erase y sesión de escritura.
- Comprobación de que el HTML no ofrece Page Program a los chips Word Program.

Consulta `docs/TEST_REPORT.md` para resultados y límites. La validación física del MX29LV160CTMC-90 sigue pendiente; por eso la UI obliga a usar SafeWrite.
