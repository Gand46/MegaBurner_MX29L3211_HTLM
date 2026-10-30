# KiCad design source — R4.9-2L-FINAL

This is intentionally a PCB-only KiCad project.

- `MegaBurner_MX26L6420_SA638_Shield.kicad_pcb` is the authoritative native
  electrical and mechanical source.
- `docs/NETLIST_AND_PINOUT.csv` is the human-readable 44-pin interface table.
- No `.sch` or `.kicad_sch` file is supplied because the former legacy
  placeholder had no wires and did not model the complete 86-position Mega R3
  mating pattern; it was not suitable for ERC or CAM regeneration.
- J1.23 is on logical rail `+3V3`, sourced only by the Mega POWER contact
  physically labelled `5V`.  The Mega must be modified so that this physical
  rail measures 3.2–3.4 V.
- The Mega POWER contact labelled `3V3` is deliberately isolated on this PCB.

Generate fabrication data from the `.kicad_pcb` file or use the already
validated Gerber/Excellon package in `fabrication/`.
