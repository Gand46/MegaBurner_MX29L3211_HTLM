MegaBurner WebSerial v0.5A NativePage256
======================================

Firmware based on v0.4C FastCRC. No risky write-path redesign was added in this release.
The project now promotes pageSize=256 bytes as the recommended MX29L3211 16-bit page profile.

Validated workflow from prior testing:
- FastRead: readBlock 16384
- FastWrite: S + X blocks + Z
- FastCRC: K0,4194304
- Baudrate: 500000
- Write block: 4096
- Recommended pageSize: 256
- Fallback pageSize: 128

Protocol:
C\n                    -> check id, returns C2F9 for MX29L3211
E\n                    -> chip erase, returns %
Rblock,size\n          -> read raw bytes
Woffset,page,size\n    -> safe write block, reset after block
S\n                    -> begin fast write transaction
Xoffset,page,size\n    -> fast write block, no reset after block
Z\n                    -> finish fast write transaction
Koffset,size\n         -> CRC32 over flash byte range, returns 8 hex chars

Recommended test sequence:
1. Connect at 500000 baud
2. Check ID
3. Load a 4 MB ROM and prepare image with mode none
4. Erase
5. Write fast with pageSize 256
6. CRC Verify rapido
7. Optional: Read/Dump with readBlock 16384 + compare dump vs prepared

Important:
- Use pageSize 128 only as fallback if pageSize 256 fails on a specific chip/board.
- Do not skip Erase when writing a different ROM image.
