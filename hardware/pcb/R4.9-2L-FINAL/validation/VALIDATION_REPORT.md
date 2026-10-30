# Validation report

Result: **PASS**

Footprints: 2; pads: 130; routed segments: 975; vias: 51.

- Pin map is complete and unique (1–44).
- Critical physical mappings P1→D7, P33→D6 and P44→D45 are preserved.
- Board bounding dimensions are 85 × 56 mm; area is 19.8% below the 106 × 56 mm reference.
- All 130 adapter/Mega pin labels are present in KiCad and the generated top-silk Gerber.
- The actual dot-matrix Gerber has no label/label or label/graphic spacing below 0.25 mm; minima are 0.295 mm and 0.260 mm.
- The top silk explicitly marks P1/P33/P44 for MX26L6420, MX29L3211 and MX29LV160CTMC.
- Adapter orientation matches the prototype after rotating its photograph 180 degrees into the standard shield top view: P1 lower-left, P22 lower-right, P23 upper-right and P44 upper-left.
- The final board contains no M3 mounting eyelets, no NPTH holes and no standalone 3V3/GND input or TP1/TP2 pair.
- The 39 large unrouted rings are intentionally isolated, unused Mega header positions retained for the complete standard mating pattern; they are not vias.
- J1 is above the Mega; minimum J1-to-Mega-header pad-edge gap is 14.342 mm.
- The serialized KiCad PCB contains all 44 J1 pads on the expected nets and the matching Mega header assignments.
- J1.23 receives logical +3V3 only from JMEGA POWER/5V; JMEGA POWER/3V3 and the two center-header 5V contacts are isolated.
- Gerber and Excellon coordinates use the same physical top view as KiCad; all 181 plated holes and 1337 generated copper pads, vias and segments passed the Y-axis transformation check; no NPTH file exists.
- Independent top-view audit SVGs were rendered back from the actual Excellon and F.Silkscreen Gerber outputs.
- Every multi-pad net, including GND, is one connected copper graph across pads, tracks and vias.
- Every one of the 51 remaining vias has valid same-net copper on both F.Cu and B.Cu; there are no one-face vias.
- The routing topology contains no dangling track endpoints, zero-length segments or duplicate segments.
- All 7 GND pads are joined by the explicit 0.50 mm routed tree; no copper pour or raster cleanup remains.
- F.Cu/B.Cu CAM contains only authoritative pads, tracks and vias, so the R4.5 source-free pour fragments cannot recur.
- Full copper DRC passed for track-track, track-pad, pad-pad, via-track, via-pad and via-via spacing; measured minima: {'track_track': 0.184, 'track_pad': 0.135, 'pad_pad': 0.69, 'via_track': 0.141, 'via_pad': 0.155, 'via_via': 0.2}.
- Signal tracks are 0.25 mm; power tracks are 0.50 mm; clearance target is 0.127 mm.
- GND is implemented as explicit routed copper on F.Cu/B.Cu, matching the editable KiCad source and Gerbers.

## Remaining physical validation

The SA638-B006 listing does not publish a certified internal continuity drawing. Before inserting a chip, verify DIP pin 1→SOP pin 1, 2→2, …, 44→44 with a multimeter.
