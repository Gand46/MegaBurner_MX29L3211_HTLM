# Development History

## Origin

The starting point was the original MegaBurner Java/SWT application and Arduino firmware for the MX29L3211. The goal was to keep the Arduino Mega programmer path, but replace the PC application with a portable HTML/Web Serial interface.

## Milestones

| Version | Purpose | Result |
|---|---|---|
| v0.1 | First Web Serial UI | Basic C/E/R/W command flow |
| v0.2 | Fixed ACK handling | First correct write/verify path |
| v0.3 | Diagnostic UI | Timing, logs, CSV, dump comparison |
| v0.4A | FastRead | Removed per-block reset; read improved dramatically |
| v0.4B | FastWrite | Added S/X/Z mode; write improved from ~17m44s to ~3–4m |
| v0.4C | FastCRC | Added Arduino-side CRC32 verification |
| v0.5A | Native page 256 | Made MX29L3211 256-byte page size the recommended profile |
| v0.5B | 1M baud test | Tested and rejected as unstable in the local setup |
| v0.5E | Final 500K | Consolidated SafeCore, FastCRC table, HTML chunk queue, Skip-FF-after-erase |
| UI-R4 | English XP UI | English interface with hover tooltips and GitHub-ready package |

## Measured local results

Representative validated results from the development session:

- Read/Dump reduced from ~11m31s to ~1m24s at 500000 baud with 16-KB read blocks.
- FastWrite with pageSize 256 validated on multiple full 4-MB non-mirrored ROMs.
- FastCRC validated against dump comparisons.

These measurements are hardware/environment dependent and should be documented again for every new build.
