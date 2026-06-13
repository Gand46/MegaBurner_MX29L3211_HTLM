# Modificación necesaria del Arduino Mega 2560 a 3.3 V

La MX29L3211 es una memoria flash de **3.3 V**. Por seguridad del chip, no debe conectarse directamente a un Arduino Mega 2560 operando a 5 V sin conversión o adaptación de niveles.

El proyecto original MegaBurner indica que la primera etapa es convertir el Mega2560 a compatibilidad de 3.3 V y remite a una guía de Adafruit. Este repositorio no reemplaza esa guía; documenta cómo afecta esa modificación al uso del programador.

## Opciones de hardware

### Opción A — Arduino Mega 2560 modificado a 3.3 V

Consiste en modificar la placa para que el ATmega2560 y sus señales de E/S trabajen a 3.3 V. Es la ruta más cercana al concepto original de MegaBurner.

Validaciones mínimas antes de conectar la MX29L3211:

- Medir VCC del ATmega2560.
- Confirmar que las salidas digitales no superen 3.3 V.
- Confirmar que el USB-serial sigue comunicando de forma estable a 500000 baudios.
- Confirmar GND común con el adaptador de flash.

### Opción B — Level shifting / adaptación de niveles

Mantiene el Mega a 5 V y adapta todas las líneas hacia la flash. Esta ruta requiere mucho cuidado porque el bus es paralelo y tiene muchas líneas:

- A0–A20.
- Q0–Q15 bidireccionales.
- WE, OE, CE, BYTE.

Debe usarse hardware capaz de manejar bus bidireccional en datos y suficiente velocidad para señales paralelas. No es recomendable usar conversores lentos pensados para I2C.

### Opción C — Shield dedicado

Replica el concepto original:

1. Adaptador SOP/TSOP para alojar la MX29L3211.
2. Shield hacia Arduino Mega con el pinout completo.
3. Ruteo corto y ordenado.
4. Alimentación 3.3 V estable.

## Imágenes de referencia del proyecto original

![Hardware original MegaBurner](assets/original-megaburner/megaburner_hw.png)

![Aplicación original MegaBurner](assets/original-megaburner/megaburner_window.png)

## Advertencias

- No alimentar la MX29L3211 con 5 V.
- No usar EPROMs de 12 V con este diseño.
- No asumir compatibilidad con chips de 64 Mbit sin revisar datasheet.
- Verificar `BYTE` en HIGH/VCC para modo 16-bit.
- Hacer siempre `Check ID` antes de borrar o escribir.
- Hacer CRC o dump completo después de escribir.

## Notas para 64 Mbit futuro

Para una memoria 64 Mbit x16 normalmente se requiere una línea adicional A21. En el mapeo actual, D49–D45 se usan como A16–A20. La línea candidata para A21 sería D44/PL5, pero esto debe confirmarse contra el datasheet del chip específico y su encapsulado.
