# User Manual

This repository is primarily documented in Spanish. See `MANUAL_USO_ES.md` for the full guide.

Quick flow:

1. Flash the firmware from `firmware/current/`.
2. Open `index.html` in Chrome/Edge.
3. Connect at the same baudrate compiled in the `.ino`.
4. Run Check ID. Expected MX29L3211 ID: `C2F9`.
5. Prepare the ROM image.
6. Erase.
7. Write.
8. Run Fast CRC Verify.
9. Optionally run full dump + compare.
