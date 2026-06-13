# User Manual — MegaBurner Web Serial v0.5E MX29L3211

## Requirements

- Arduino Mega 2560 with the v0.5E firmware.
- MX29L3211 wired according to `docs/PINOUT_MX29L3211_MEGA2560.md`.
- Chrome or Edge with Web Serial support.
- A ROM/image file up to 4,194,304 bytes.

## Recommended settings

| Setting | Recommended value |
|---|---:|
| Baudrate | 500000 |
| Chip size | 4194304 bytes |
| Write mode | Fast |
| Write block | 4096 bytes |
| Page size | 256 bytes |
| Read block | 16384 bytes |
| Verification | Fast CRC first, Full Verify when needed |
| Skip FF | Off by default; enable only after a successful Erase |

## Standard workflow

1. Open `web/index.html`.
2. Click **Connect**.
3. Click **Check ID**. A correct MX29L3211 should return `C2F9`.
4. Select a ROM/image file.
5. Click **Prepare image**.
6. Click **Erase**.
7. Click **Write**.
8. Click **Fast CRC**.
9. Optionally click **Read/Dump** and **Compare** for full byte-by-byte verification.

## Fill modes

- **Mirror/repeat to 4 MB**: repeats a smaller image until the chip size is filled.
- **Pad with FF to 4 MB**: pads unused space with erased flash value `0xFF`.
- **Do not modify**: uses the file exactly as loaded. Recommended for exact 4-MB images.

## Tooltips

Every button has a hover tooltip using the HTML `title` attribute. Hold the mouse over a button to see its function.

## Safety rules

- Always erase before writing a different image.
- Keep baudrate identical to the compiled firmware.
- Leave `writeBlock=4096` for Arduino Mega SRAM safety.
- Use `pageSize=256` for MX29L3211; keep `128` only as fallback.
- Do not enable Skip FF unless Erase completed successfully in the same session.
