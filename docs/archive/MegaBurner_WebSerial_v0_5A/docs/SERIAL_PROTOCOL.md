# Serial Protocol

The Web Serial application and Arduino firmware use a compact ASCII command protocol. The firmware v0.5E is compiled for 500000 baud by default.

## Commands

| Command | Direction | Meaning |
|---|---|---|
| `V
` | PC → Arduino | Firmware version/capabilities |
| `C
` | PC → Arduino | Read flash ID; MX29L3211 expected response is `C2F9` |
| `E
` | PC → Arduino | Chip erase |
| `Rblock,size
` | PC → Arduino | Read `size` bytes from block index |
| `Woffset,page,size
` | PC → Arduino | Safe legacy write block |
| `S
` | PC → Arduino | Begin FastWrite session |
| `Xoffset,page,size
` | PC → Arduino | FastWrite block |
| `Z
` | PC → Arduino | Finish FastWrite session and reset flash |
| `Koffset,size
` | PC → Arduino | Fast CRC32 from flash |
| `Q
` | PC → Arduino | Read status register |
| `L
` | PC → Arduino | Clear status register |

## ACK / error tokens

| Token | Meaning |
|---|---|
| `&` | Arduino is ready to receive a binary block |
| `%` | Operation completed successfully |
| `!P` | Program fail / status Q4 |
| `!E` | Erase fail / status Q5 |
| `!T` | Busy/status timeout |
| `!R` | Timeout receiving block |
| `!C` | Invalid command/parameters |
| `!B` | Invalid block/larger than MAXW |
| `!A` | Alignment error |

## Final tested profile

- Baudrate: 500000
- Write block: 4096
- Page size: 256
- Read block: 16384
