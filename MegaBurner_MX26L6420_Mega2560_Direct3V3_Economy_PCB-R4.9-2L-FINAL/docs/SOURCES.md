# Research and source references

- Supplied physical-interface reference: `MegaBurner_MultiChip_v0_7A_ProfileSafe.zip`.
- Original MegaBurner repository and MX29L3211 wiring baseline: https://github.com/maximaas/MegaBurner
- Original low-level MX29L3211 source confirming PH4/WE and PH3/BYTE: https://github.com/maximaas/MegaBurner/blob/master/arduino/MX29L3211.cpp
- MX26L6420 data sheet (VCC 2.7–3.6 V; SOP44 pinout): https://www.wolfgangrobel.de/electronics/datasheets/eprom/MX26L6420.pdf
- Microchip ATmega2560 product/data-sheet page (16 MHz specified at 4.5–5.5 V): https://www.microchip.com/en-us/product/ATmega2560
- Arduino Mega 2560 official specifications (stock 5 V/16 MHz; 3.3 V pin 50 mA): https://store.arduino.cc/products/arduino-mega-2560-rev3
- Arduino Mega 2560 official full pinout (POWER `3V3`/`5V` and the two center-header `5V` contacts beside D22/D23): https://docs.arduino.cc/resources/pinouts/A000067-full-pinout.pdf
- Arduino Mega 2560 official schematic (all physical `5V` contacts belong to the same stock +5V rail): https://docs.arduino.cc/resources/schematics/A000067-schematics.pdf
- Published Mega 2560 shield footprint used to cross-check header coordinates: https://github.com/speeduino/kicad-parts/blob/master/Shields/Shields.pretty/Arduino_Mega2560_Shield.kicad_mod
- SA638-B006/SDP-UNV-44PSOP dimensions (SOP pitch 1.27 mm; DIP pitch 2.54 mm; 600 mil width): https://www.pcb-hero.com/products/412353
- Independent SA638-B006 description (44-pin SOP to 44-pin DIL): https://www.equinox-tech.com/products/details.asp?ID=512
- Supplied photos: `reference/01-SA638-B006.jpg` and `reference/02-Mega-prototype.jpg`.
- Placement close-up supplied for R3: `reference/03-adapter-over-Mega.jpg`.
- Keyestudio-style outline photo used as the original-size reference: `reference/04-keyestudio-KS0005-outline.jpeg`.
- Official Keyestudio KS0005 dimensions (106 × 56 × 19 mm): https://wiki.keyestudio.com/index.php/Ks0005_keyestudio_MEGA_ProtoShield_V3_with_Tiny_Breadboard
- JLCPCB dimension guidance (Gerbers are auto-detected; irregular boards may receive manual manufacturability review): https://jlcpcb.com/help/article/pcb-dimensions
- JLCPCB order guidance for two-layer boards and detected dimensions: https://jlcpcb.com/help/article/how-do-i-place-an-order
- PCBWay standard capability guidance recommends traces/spacing above 0.15 mm where routing density permits: https://www.pcbway.com/capabilities.html

The photos and listings establish the mechanical intent, but not a certified internal continuity diagram. Meter verification remains mandatory.
