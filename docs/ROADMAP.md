# Roadmap

## Completed

- Java/SWT desktop application replaced by HTML/Web Serial.
- Stable 500000 baud profile.
- FastRead, FastWrite, FastCRC.
- Native 256-byte MX29L3211 page profile.
- Status-aware firmware errors.
- English UI with tooltips.

## Future work

### 64-Mbit flash support

A 64-Mbit x16 target generally requires one extra word address line compared with MX29L3211. In the current Arduino Mega port layout, `PL5 / D44` is available as a candidate future `A21` line. `PL6 / D43` and `PL7 / D42` remain possible future expansion lines.

Important: the MX29L3211 SOP44 physical pin 44 is A20. Future 64-Mbit support must be based on the exact chip pinout and adaptor board, not on assumptions.

### Sector mode

- CRC per sector.
- Erase only changed sectors.
- Write only changed sectors.

### Additional devices

- Evaluate 29GL064/29LV640 style 64-Mbit chips.
- Add chip profile selection.
- Keep voltage and command-set validation per datasheet.

### Hardware refinement

- Dedicated shield/adaptor PCB.
- Safer 3.3-V power/level strategy.
- Optional test pads for A21/A22/A23.
