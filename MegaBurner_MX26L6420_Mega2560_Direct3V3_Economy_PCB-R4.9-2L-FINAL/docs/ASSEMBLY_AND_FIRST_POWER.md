# Assembly and first-power checklist — direct 3.3 V R4.9-2L-FINAL economy board

1. Install the complete 86-position Mega R3 stacking-header set. There are no C1/C2 footprints and no separate power-input connector.
2. With no adapter installed and power off, verify J1 pin 23 is continuous to the Mega POWER contact physically marked `5V`.
3. Confirm open circuit between J1 pin 23 and each of POWER/`3V3`, D22 and D9. Also verify J1 pin 23 is not shorted to J1 pin 13 or 32/GND.
4. Power the modified Mega and confirm POWER/`5V`, J1 pin 23 and IOREF are all 3.2–3.4 V relative to J1 pin 13 or 32. If the contact marked `5V` measures near 5 V, disconnect immediately and do not install a memory.
5. Confirm that no GPIO, pull-up, USB-serial interface or peripheral can restore a 5 V domain on any used line.
6. Power off. With the shield viewed from above and the Mega USB/power connectors at the left, place the adapter horizontally with pin 1 at the lower-left PCB mark. The opposite corners are pin 22 lower-right, pin 23 upper-right and pin 44 upper-left. Solder all 44 male pins into J1.
   This final board has no mounting eyelets: place an insulated flat support beneath the shield whenever the clamp socket is opened or closed.
7. With no flash inserted, continuity-test J1 pin 1→adapter SOP contact 1 through pin 44→contact 44. Check for adjacent shorts.
8. Recheck J1 pins 13 and 32 to GND, J1 pin 23 to POWER/`5V`, and POWER/`3V3` as isolated. J1 pin 23 must never see 5 V.
9. Pay special attention to J1 pin 25/Q12/D26/PA4, which was contact-sensitive in the prior hand-wired prototype.
10. Insert the MX26L6420 with pin 1 aligned, power up, and run Read ID before any erase/write command.
11. Select the matching MX26L6420, MX29L3211 or MX29LV160CTMC profile before any erase/write operation; use the printed P1/P33/P44 legend as a physical cross-check.

Never insert/remove the memory or operate the clamp while powered.
