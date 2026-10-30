# PCB R4.9 — conexiones, potencia y uso con firmware v0.7A

**Versión vigente:** MultiChip v0.7A ProfileSafe + shield R4.9-2L-FINAL. **Fuente física:** [KiCad editable](../hardware/pcb/R4.9-2L-FINAL/kicad/MegaBurner_MX26L6420_SA638_Shield.kicad_pcb) y [CSV original de red/pinout](../hardware/pcb/R4.9-2L-FINAL/docs/NETLIST_AND_PINOUT.csv).

> **NO USAR CON MEGA 2560 ESTÁNDAR DE 5 V.** En esta PCB `J1.23` recibe la red `+3V3` desde el contacto del Mega marcado **`5V`**, que debe medir **3,2–3,4 V** después de la modificación. `POWER/3V3` del Mega permanece **aislado**. No hay regulador, level-shifter ni protección 5 V en el PCB.

## Dependencia hardware

- Fuente de autoridad eléctrica: KiCad PCB R4.9, no el viejo pinout v0.5A ni las fotos históricas.
- Tensiones en VCC de chip, IOREF y pines de I/O: rango 3,3 V, con comprobación de posibles 5 V desde USB-serie, reset, periféricos o conectores. La alimentación del Mega modificado debe tener corriente suficiente y estabilidad verificada; no se presupone que el pin `3V3` de un Mega estándar alimente esta placa.
- Un Mega 2560 de 16 MHz a 3,3 V requiere validación específica de funcionamiento y de sus condiciones de frecuencia/tensión. No atribuir estabilidad a una conversión de voltaje sin ensayo.
- La PCB carece de C1/C2 y de desacoplo integrado adicional; verificar el desacoplo real en el conjunto Mega + adaptador y emplear el apropiado en el montaje conforme al datasheet.

## Pines de función variable

| Pin SOP44 / J1 | Dato físico Mega | MX29L3211 | MX26L6420MC-90 | MX29LV160CTMC-90 |
|---:|---|---|---|---|
| 1 | D7 / PH4 | WE# | A21 | RESET# |
| 33 | D6 / PH3 | BYTE# | WE# | BYTE# |
| 44 | D45 / PL4 | A20 | A20 | WE# |

La etiqueta de red del cobre de R4.9 (`A21` para J1.1, `WE` para J1.33, `A20` para J1.44) corresponde al perfil **MX26L6420**, no obliga al resto de chips a usar la misma función. El firmware cambia los roles dinámicamente; nunca seleccionar un perfil incompatible.

## Alimentación

| J1 | Señal real | Origen/destino |
|---:|---|---|
| 13 | GND | GND de Mega |
| 23 | VCC 3,3 V | POWER/`5V` del Mega modificado, red `+3V3` del shield |
| 32 | GND | GND de Mega |
| 38 | A12 | A12 / PK4 del Mega (no VCC) |

## Pinout completo, los 44 pines

La tabla siguiente se genera del CSV R4.9 aportado, no del documento v0.5A.

| J1 | Mega | Puerto AVR | Red PCB | MX26L6420 | MX29L3211 | MX29LV160CTMC |
|---:|---|---|---|---|---|---|
| 1 | D7 | PH4 | A21 | A21 | WE# | RESET# |
| 2 | D47 | PL2 | A18 | A18 | A18 | A18 |
| 3 | D48 | PL1 | A17 | A17 | A17 | A17 |
| 4 | A7 | PF7 | A7 | A7 | A7 | A7 |
| 5 | A6 | PF6 | A6 | A6 | A6 | A6 |
| 6 | A5 | PF5 | A5 | A5 | A5 | A5 |
| 7 | A4 | PF4 | A4 | A4 | A4 | A4 |
| 8 | A3 | PF3 | A3 | A3 | A3 | A3 |
| 9 | A2 | PF2 | A2 | A2 | A2 | A2 |
| 10 | A1 | PF1 | A1 | A1 | A1 | A1 |
| 11 | A0 | PF0 | A0 | A0 | A0 | A0 |
| 12 | D9 | PH6 | CE | CE# | CE# | CE# |
| 13 | GND | — | GND | GND | GND | GND |
| 14 | D16 | PH1 | OE | OE# | OE# | OE# |
| 15 | D37 | PC0 | Q0 | Q0 | Q0 | Q0 |
| 16 | D22 | PA0 | Q8 | Q8 | Q8 | Q8 |
| 17 | D36 | PC1 | Q1 | Q1 | Q1 | Q1 |
| 18 | D23 | PA1 | Q9 | Q9 | Q9 | Q9 |
| 19 | D35 | PC2 | Q2 | Q2 | Q2 | Q2 |
| 20 | D24 | PA2 | Q10 | Q10 | Q10 | Q10 |
| 21 | D34 | PC3 | Q3 | Q3 | Q3 | Q3 |
| 22 | D25 | PA3 | Q11 | Q11 | Q11 | Q11 |
| 23 | 5V | — | +3V3 | VCC | VCC | VCC |
| 24 | D33 | PC4 | Q4 | Q4 | Q4 | Q4 |
| 25 | D26 | PA4 | Q12 | Q12 | Q12 | Q12 |
| 26 | D32 | PC5 | Q5 | Q5 | Q5 | Q5 |
| 27 | D27 | PA5 | Q13 | Q13 | Q13 | Q13 |
| 28 | D31 | PC6 | Q6 | Q6 | Q6 | Q6 |
| 29 | D28 | PA6 | Q14 | Q14 | Q14 | Q14 |
| 30 | D30 | PC7 | Q7 | Q7 | Q7 | Q7 |
| 31 | D29 | PA7 | Q15 | Q15 | Q15 | Q15 |
| 32 | GND | — | GND | GND | GND | GND |
| 33 | D6 | PH3 | WE | WE# | BYTE# | BYTE# |
| 34 | D49 | PL0 | A16 | A16 | A16 | A16 |
| 35 | A15 | PK7 | A15 | A15 | A15 | A15 |
| 36 | A14 | PK6 | A14 | A14 | A14 | A14 |
| 37 | A13 | PK5 | A13 | A13 | A13 | A13 |
| 38 | A12 | PK4 | A12 | A12 | A12 | A12 |
| 39 | A11 | PK3 | A11 | A11 | A11 | A11 |
| 40 | A10 | PK2 | A10 | A10 | A10 | A10 |
| 41 | A9 | PK1 | A9 | A9 | A9 | A9 |
| 42 | A8 | PK0 | A8 | A8 | A8 | A8 |
| 43 | D46 | PL3 | A19 | A19 | A19 | A19 |
| 44 | D45 | PL4 | A20 | A20 | A20 | WE# |

## Revisión obligatoria previa a la primera alimentación

1. **Todo apagado y sin flash:** comprobar continuidad de J1.23 con contacto Mega POWER/`5V` y aislamiento de J1.23 respecto de `POWER/3V3`, D22, D9, GND. Comprobar J1.13 y J1.32 a GND.
2. Medir en el Mega modificado sin memoria: contacto marcado `5V` = **3,2–3,4 V**, IOREF ≈ 3,3 V y GPIO sin tensión que exceda la admitida por la flash. Si alguna línea presenta 5 V, **desconectar**.
3. Verificar estabilidad de alimentación bajo carga, interfaz USB-serie y reloj del MCU con el Mega convertido a 3,3 V. Confirmar ausencia de alimentación cruzada. El PCB no incorpora desacoplo adicional.
4. **Apagado:** montar adaptador SA638-B006 orientado PIN1 abajo a la izquierda en vista superior de shield (USB del Mega a la izquierda). Pines esquina: 1 inferior izquierda, 22 inferior derecha, 23 superior derecha, 44 superior izquierda.
5. Comprobar con polímetro todas las conexiones **DIP44→SOP44** del adaptador real, continuidad y cortos adyacentes. No asumir mapeo interno 1:1 sin medida.
6. Apoyar shield en soporte plano aislado para accionar la pinza (R4.9 carece de ojales de fijación); verificar altura libre respecto a los ICSP.
7. Insertar la flash sólo sin alimentación, según orientación correcta. Conectar y elegir el perfil correcto, después `Check ID`. Cualquier fallo = **no borrar/escribir**.
8. Erase/Write/CRC/doble dump únicamente tras superar todas las comprobaciones y teniendo copia de seguridad de la imagen. Para MX29LV160CT usar SafeWrite y reconocer que falta validación física.

**Lo que no está probado por integración documental:** mediciones reales de tensión/corriente, funcionamiento del regulador/USB-serie modificado, continuidad del socket adquirido, grabado sobre shield R4.9 fabricado, DRC/ERP desde KiCad nativo en este entorno. Ver [matriz de validación](INTEGRATION_VALIDATION.md).
