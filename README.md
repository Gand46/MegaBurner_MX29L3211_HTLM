# MegaBurner Web Serial MX29L3211

A browser-based replacement for the original MegaBurner Java/SWT desktop utility, focused on programming the **Macronix MX29L3211 32-Mbit / 4-MB parallel flash** with an **Arduino Mega 2560**.

This repository keeps the proven MegaBurner hardware/firmware approach and replaces the PC-side Java/RXTX/SWT dependency stack with a single HTML application that uses the Web Serial API.

## Current release

**v0.5E MX29L3211**

- 500000 baud stable profile.
- FastWrite protocol: `S` + repeated `X` blocks + `Z` final reset.
- FastRead without per-block flash reset.
- FastCRC command `Koffset,size` calculated by the Arduino firmware.
- Native MX29L3211 page size: 256 bytes / 128 words.
- Status-aware polling with program/erase fail and timeout errors.
- Optional Skip-FF-after-erase optimization.
- English Windows XP-style UI with button hover tooltips.

## Quick start

1. Compile and upload `firmware/current/MegaBurner.ino` to an Arduino Mega 2560.
2. Open `web/index.html` in Chrome or Edge.
3. Connect at **500000 baud**.
4. Click **Check ID**. Expected ID: `C2F9`.
5. Load a ROM/image, prepare it, erase the chip, write, and run **Fast CRC**.

## Project origin and credits

This work is derived from and documents a PC-side port of `maximaas/MegaBurner`. MegaBurner states that it copied/used the 16-bit flash programming feature from `sanni/cartreader`, used MEEPROMMER-style PC-to-Mega2560 serial communication, and used wiring compatible with the cartreader expansion shield.

See [docs/CREDITS.md](docs/CREDITS.md) for acknowledgements and source references.

## Repository layout

```text
firmware/current/     Current v0.5E Arduino firmware
firmware/legacy/      Firmware evolution snapshots
web/index.html        Current English Web Serial UI
web/releases/         HTML evolution snapshots
docs/                 User manual, protocol, pinout, development history
hardware/             Pinout CSV and wiring notes
examples/             Placeholder for test logs and ROM-free examples
```

## Hardware warning

The MX29L3211 is a **3.3 V flash device**. Use a proper 3.3 V supply and verify your shield/level strategy before connecting hardware. The Arduino Mega 2560 itself is a 5 V board.

## Status

Validated in the project tests with multiple 4-MB non-mirrored ROM images at 500000 baud. The 1000000 baud variant was tested and found unstable in this setup, so the final profile remains 500000 baud.
