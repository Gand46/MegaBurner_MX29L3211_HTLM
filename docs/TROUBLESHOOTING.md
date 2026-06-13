# Troubleshooting

## Check ID does not return C2F9

- Confirm baudrate matches the compiled firmware.
- Confirm wiring of CE/OE/WE/BYTE and D0-D15.
- Confirm the flash is powered at 3.3 V.
- Confirm BYTE is high for x16 mode.

## Write succeeds but verify fails with bits stuck at 0

This usually means the chip was not erased before writing. NOR flash can program 1 → 0, but cannot program 0 → 1 without erase.

## 1000000 baud is unstable

Use the final 500000 baud profile. This project rejected 1M as unstable after read failures in local tests.

## Skip FF writes old data

Skip FF is only safe after Erase OK in the same session. Leave it off unless you understand the image layout.
