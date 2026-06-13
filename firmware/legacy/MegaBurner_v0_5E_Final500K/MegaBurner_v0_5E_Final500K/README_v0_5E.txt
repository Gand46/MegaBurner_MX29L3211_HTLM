MegaBurner WebSerial v0.5E Final500K
====================================

Final consolidated HTML-port branch for MX29L3211 at the stable 500000 baud profile.
This release intentionally stays at 500000 baud because 1000000 baud was unstable in real testing.

Included files:
- MegaBurner_v0_5E_Final500K.ino
- MX29L3211.cpp
- MX29L3211.h

HTML application:
- megaburner_webserial_v0_5E_final500k.html

Validated baseline inherited from prior testing:
- v0.4A FastRead
- v0.4B FastWrite S/X/Z
- v0.4C FastCRC K
- v0.5A NativePage256
- Stable baudrate: 500000
- Recommended pageSize: 256 bytes / 128 words
- Fallback pageSize: 128 bytes / 64 words
- writeBlock: 4096
- readBlock: 16384

New consolidated changes:

1. v0.5B_500K SafeCore
   - Firmware capabilities command V.
   - Status register command Q.
   - Clear status command L.
   - Status-aware busy polling for program/erase.
   - Error responses:
     %  = OK
     !P = program fail / Q4
     !E = erase fail / Q5
     !T = timeout waiting for Q7 ready
     !R = receive timeout
     !C = command/parameter error
     !B = block too large/invalid
     !A = alignment error

2. v0.5C FastCRC Table
   - CRC32 moved from bitwise algorithm to a 16-entry nibble table in PROGMEM.
   - Keeps RAM usage low and should improve K0,4194304 verify time.

3. v0.5D HTML Chunk Queue
   - HTML RX queue uses Uint8Array chunks instead of pushing every byte into a JS Array.
   - Writing uses subarray() views to avoid unnecessary copies.
   - Better error handling for firmware ! codes.
   - Handles already-open port case more cleanly.

4. v0.5E Skip FF After Erase
   - HTML can skip 0xFF pages after a confirmed Erase in the same session.
   - Skip FF is disabled automatically after each Write attempt.
   - Safe for blank/padded images, useful for ROMs smaller than 4 MB filled with FF.
   - Do not use Skip FF unless the tool performed Erase successfully in the same session.

Protocol:
V\n                    -> version/capabilities line
C\n                    -> check ID, returns C2F9 for MX29L3211
E\n                    -> chip erase, returns % or !code
Rblock,size\n          -> read raw bytes
Woffset,page,size\n    -> safe write block, reset after block
S\n                    -> begin fast write transaction
Xoffset,page,size\n    -> fast write block, no reset after block
Z\n                    -> finish fast write transaction
Koffset,size\n         -> CRC32 over flash byte range, returns 8 hex chars
Q\n                    -> status register, returns 4 hex chars
L\n                    -> clear status register, returns %

Recommended test sequence:
1. Open HTML v0.5E.
2. Connect at 500000 baud.
3. Click Firmware V.
4. Check ID, expected C2F9.
5. Load ROM/image.
6. Prepare image.
7. Erase.
8. Write fast with pageSize 256.
9. CRC Verify rápido.
10. Optional deep validation: Read/Dump with readBlock 16384 + Compare dump vs preparada.

Recommended parameters:
- baudrate: 500000
- writeMode: fast
- writeBlock: 4096
- pageSize: 256
- readBlock: 16384
- Skip FF: enabled only if Erase OK in same session

Notes:
- 1000000 baud is not recommended because it was unstable in testing.
- Do not use pageSize 512; MX29L3211 native page program is 256 bytes / 128 words.
- Do not skip Erase when writing a different image.
- If firmware reports !P, !E or !T, stop and use Status Q and a full erase/write/verify cycle.
