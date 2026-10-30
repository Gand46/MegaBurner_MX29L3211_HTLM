# Mechanical placement validation — R4.9-2L-FINAL economy board

Coordinate origin is the upper-left reference used by the generated PCB. All dimensions are in millimetres.

## Adapter position over the Mega

| Item | X range / coordinate | Y range / coordinate |
|---|---:|---:|
| Finished economy shield | 21.00–106.00 | 0.00–56.00 |
| Projected Arduino Mega PCB | 4.00–105.60 | 1.00–54.34 |
| SA638-B006 body envelope | 26.17–84.17 | 12.00–44.00 |
| J1 top row | 28.50–81.84 | 20.38 |
| J1 bottom row | 28.50–81.84 | 35.62 |

The complete 58 × 32 mm adapter envelope and all 44 J1 holes are above the Arduino Mega footprint. The adapter is horizontal, as in the supplied photograph; there is no right-side adapter extension.

## Pin-number orientation

| Corner | J1 pin | Coordinate |
|---|---:|---:|
| Lower-left | 1 | X=28.50, Y=35.62 |
| Lower-right | 22 | X=81.84, Y=35.62 |
| Upper-right | 23 | X=81.84, Y=20.38 |
| Upper-left | 44 | X=28.50, Y=20.38 |

Pitch is 2.54 mm along each row and row separation is 15.24 mm (600 mil). This matches the supplied prototype after rotating its photograph 180 degrees into the standard Arduino shield top view (USB/power connectors at left and D22–D53 at right).

## Clearances and mechanical support

- Minimum calculated copper-edge distance from any J1 pad to any Mega header pad: 14.342 mm.
- Adapter body clearance from the upper header pad edge: approximately 7.54 mm.
- Adapter body clearance from the lower header pad edge: approximately 6.88 mm.
- This final fabrication revision contains zero M3 mounting eyelets and zero NPTH holes. The three former 3.20 mm holes were deliberately removed.
- Only plated holes remain: J1, the complete 86-position Arduino Mega R3 mating headers and routed/layer-transition vias. The added POWER NC hole is electrically unconnected and is not an input.
- Use stacking headers tall enough to keep the shield underside clear of both Mega ICSP 2×3 headers; verify at least 2 mm physical clearance before operating the ZIF clamp.
- Support the shield on a flat insulated fixture when operating the clamp socket so its force is not carried only by the Mega headers.

## Economy-outline validation

- Reference board: 106 × 56 = 5,936 mm².
- R4.9-2L-FINAL board: 85 × 56 = 4,760 mm².
- Removed area: 1,176 mm² (19.8%).
- The right edge and all Mega/J1 coordinates remain unchanged; only the electrically unused X=0…21 mm strip is removed.
- A rectangular outline is intentionally preferred over internal cutouts: board quotation systems use detected outer dimensions and irregular profiles may require manual manufacturability review.

The 58 × 32 mm body envelope is based on the researched product dimensions and photographs, not a manufacturer-controlled mechanical drawing. Before ordering a production batch, measure the actual purchased adapter with calipers and compare its male-pin pitch, row spacing and outer body against these values.
