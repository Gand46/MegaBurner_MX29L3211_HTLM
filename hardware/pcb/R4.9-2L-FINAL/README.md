# MegaBurner MultiChip — Mega 2560 Direct 3.3 V → SA638-B006 — Economy PCB R4.9-2L-FINAL

Fabrication-ready **single-board shield** for the physical interface used by MegaBurner MultiChip v0.7A. The SA638-B006 / SDP-UNIV-44PS adapter is soldered directly into J1; there is no second DIP socket, level translator, onboard regulator or decoupling capacitor on this shield.

## Electrical architecture

- Direct 3.3 V point-to-point connection from the modified Arduino Mega to all address, data and control pins.
- J1 pin 23 receives logical `+3V3` from the Mega POWER contact physically labelled `5V`. On the required modified Mega, that complete rail delivers 3.3 V.
- The Mega POWER contact physically labelled `3V3` is present mechanically but deliberately isolated: it has no PCB net, trace or copper bridge.
- The two center-header `5V` contacts beside D22/D23 remain isolated on this shield; D22 and D23 retain their Q8/Q9 data functions.
- J1 pins 13 and 32 use the explicit 0.50 mm routed GND tree.
- There is no external power input and no separate `3V3/GND` test-point header; the modified Mega is the only supply source for the shield.
- The shield contains no C1/C2 or other added decoupling, by design requirement.
- Critical firmware mapping is preserved: J1 pin 1/A21 → D7/PH4 and J1 pin 33/WE# → D6/PH3.
- The three variable positions remain physically invariant: P1→D7/PH4, P33→D6/PH3 and P44→D45/PL4. Their functions are selected by the MX26L6420, MX29L3211 or MX29LV160CTMC profile.
- The complete table is in `docs/NETLIST_AND_PINOUT.csv`.

## Mandatory Mega modification assumptions

This PCB intentionally has **no 5 V protection and no power conversion**. It assumes the proven hardware modification in which the Mega's onboard regulator is removed and the complete rail physically labelled `5V` is driven directly at 3.3 V, so the entire Mega board and every used GPIO operate in the 3.3 V domain. Before installing the adapter, verify all of the following:

1. The ATmega2560 supply and `IOREF` are 3.3 V.
2. The Mega POWER contact marked `5V` measures 3.2–3.4 V; it must not carry 5 V.
3. No GPIO, pull-up, USB-serial interface, shield or peripheral can drive a used pin above 3.6 V.
4. J1 pin 23 measures 3.2–3.4 V relative to J1 pin 13 or 32 under load.
5. J1 pin 23 is continuous only to POWER/`5V`, while POWER/`3V3`, D22 and D9 remain isolated from it.
6. The shield is never connected to a stock 5 V Mega or to a second external supply.

## Economy outline and mechanical placement

- Finished bounding size is 85 × 56 mm (X=21…106 mm in the retained Mega coordinate system).
- This removes 1,176 mm², or 19.8%, from the 106 × 56 mm KS0005 reference and brings the maximum dimension below 100 mm.
- A simple rectangular CNC-routed outline is used to avoid the manual review that some manufacturers apply to irregular profiles.
- The adapter is horizontal and centered above the Arduino Mega PCB, between its upper and lower shield headers.
- The 58 × 32 mm adapter envelope is fully inside the projected 101.6 × 53.34 mm Mega area; there is no lateral PCB extension.
- The prototype photograph is rotated 180 degrees relative to the standard shield top view. After normalizing that view, J1 pin 1 is lower-left, pin 22 lower-right, pin 23 upper-right and pin 44 upper-left.
- J1 has 2.54 mm pin pitch and 15.24 mm/600 mil row spacing; its 44 holes do not collide with any Mega header pad.
- The complete Mega R3 header pattern is present, including the electrically unconnected POWER-header NC position. No M3 mounting eyelets, NPTH holes or standalone `3V3/GND` input/test-point holes are included.
- Because there are no mounting eyelets, support the assembled shield on a flat insulated fixture while operating the clamp socket; do not transfer clamp force through the Arduino headers alone.

## Routing and silkscreen

- All 43 electrical nets, including GND, are explicitly routed only on F.Cu/B.Cu. No copper pour is used; this removes the source-free fragments found in R4.5 while preserving the tested power path.
- The routing audit removed eight obsolete one-face GND stitching vias and the complete residual GND spur left by the former C1/C2 routing anchors. The 51 remaining vias are functional F.Cu↔B.Cu transitions with same-net copper on both faces.
- The 39 large plated rings without tracks are intentionally unused Mega R3 header positions, not vias. They remain electrically isolated so a complete standard header set can be fitted mechanically.
- Signal tracks are 0.25 mm; the logical +3V3 supply from POWER/`5V` is 0.50 mm; design clearance is 0.127 mm (5 mil).
- The top silk contains 130 pin labels: every J1 pin and every Mega R3 header position, including POWER NC.
- Three compatibility lines identify P1/P33/P44 for MX26L6420, MX29L3211 and MX29LV160CTMC.
- The lower-left J1 corner is identified redundantly by the rectangular pad, triangular body mark, neutral `1/D7` label and `PIN 1` legend.
- The top silk states `MEGA MOD: 5V=3V3`; the POWER labels read `3V3/NC` and `5V/+3V3`.
- Labels are present both in the native KiCad board and as actual 0.12 mm dot-matrix artwork in the supplied top-silkscreen Gerber.
- The manufactured Gerber is checked for text–text, text–graphic and text–edge spacing; every artwork gap is at least 0.25 mm.

## Files

- `kicad/` — native KiCad PCB/project plus `PCB_ONLY_SOURCE.md`; no misleading legacy schematic placeholder is included.
- `fabrication/MegaBurner_R4_9_2L_FINAL_Gerbers.zip` — manufacturing package.
- `fabrication/BOM.csv` and `CPL.csv` — assembly data.
- `docs/BOARD_OUTLINE_COORDINATES.csv` — exact 85 × 56 mm Edge.Cuts vertices.
- `docs/SILKSCREEN_PIN_LABELS.csv` — all 130 pin labels plus the three compatibility lines, with position and rotation.
- `preview/PCB_preview.svg` — routed two-layer preview.
- `preview/FAB_top_drill_from_Excellon.svg` and `preview/FAB_top_silkscreen_from_Gerber.svg` — independent top-view audits rendered from the actual manufacturing files.
- `reference/` — the four supplied mechanical reference photographs.
- `validation/` — connectivity, clearance, routing-topology and fabrication checks.

## PCB order

- 85 × 56 mm rectangle, **two copper layers**, FR-4 1.6 mm, 1 oz copper.
- Manufacturer capability of at least 0.25/0.127 mm track/space and 0.12 mm silkscreen strokes.
- ENIG is preferred for the directly soldered adapter pins; HASL lead-free is acceptable.

Supplied firmware archive SHA-256: `a65bfd19dab3b477fb63223cf6ef51f8739da77df6eb2e7b6f924595446024d9`.
