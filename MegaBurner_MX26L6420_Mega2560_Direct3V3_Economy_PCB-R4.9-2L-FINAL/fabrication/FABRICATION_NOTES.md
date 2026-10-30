# Fabrication notes — direct 3.3 V R4.9-2L-FINAL economy board

- Finished board bounding box: 85 × 56 mm; simple rectangular Edge.Cuts from X=21 to X=106 mm in retained Mega coordinates.
- 2-layer FR-4, 1.6 mm, 1 oz copper.
- Signal traces: 0.25 mm; power traces: 0.50 mm; design clearance: 0.127 mm (5 mil).
- Functional layer-transition vias: 51 at 0.80/0.40 mm; 181 total plated drills including Mega/J1; no mounting holes and no NPTH drill file.
- No GND stitching vias are used because the board has no copper pours. Every remaining via carries its net between valid copper on both F.Cu and B.Cu.
- No external power connector or standalone 3V3/GND test-point holes.
- Logical +3V3 enters only through the Mega POWER contact physically labelled `5V`; the required modified Mega must deliver 3.2–3.4 V on that rail. The POWER contact labelled `3V3` is isolated on this PCB.
- The center-header 5V contacts beside D22/D23 remain isolated; D22/D23 stay on Q8/Q9.
- No copper pours or negative-polarity raster cleanup. GND is an explicit 0.50 mm routed tree and the Gerbers contain only pads, tracks and vias.
- J1 row pitch: 15.24 mm (600 mil); pin pitch: 2.54 mm; standard DIP44 numbering.
- J1 is for direct soldering of the SA638-B006 male pins; do not add another DIP socket.
- J1 orientation in standard shield top view: pin 1 lower-left, pin 22 lower-right, pin 23 upper-right, pin 44 upper-left.
- Gerber and Excellon coordinates are exported in a Cartesian +Y-up frame using `Yfab = 56.00 - Ysource`; this preserves the same physical top view as the native KiCad/SVG project.
- The 58 × 32 mm SA638-B006 keepout is centered above the Mega and remains inside its projected PCB area.
- No mechanical eyelets are present. During assembly and use, place a flat insulated support under the shield before opening or closing the clamp socket.
- F.Silkscreen includes 130 pin labels, three P1/P33/P44 device-compatibility lines, `PIN 1` and `MEGA MOD: 5V=3V3`, all drawn with a 0.12 mm aperture and validated with at least 0.25 mm between independent artwork envelopes. Confirm the selected manufacturer accepts 0.12 mm silkscreen strokes.
