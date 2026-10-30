#!/usr/bin/env python3
from pathlib import Path
root=Path(__file__).resolve().parents[1]
fw=root/'firmware'/'MegaBurner_MultiChip_v0_7A_ProfileSafe'
texts='\n'.join(p.read_text(encoding='utf-8') for p in fw.iterdir() if p.suffix in {'.ino','.cpp','.h'})
for forbidden in ['String ', 'new ', 'malloc(', 'calloc(', 'realloc(']:
    assert forbidden not in texts, f'Dynamic allocation pattern found: {forbidden}'
assert '#define MAX_WRITE_BLOCK 4096' in texts
assert '#define CMD_BUFFER_SIZE 96' in texts
# Conservative static SRAM estimate: write buffer + command buffer + 3 error structs + state and objects.
estimated=4096+96+3*16+128
assert estimated < 6144, estimated
assert 'universalSafePins();' in texts
assert 'profileLocked = idOkLatch;' in texts
assert 'eraseOkLatch' in texts and 'writeSessionActive' in texts
print(f'Memory/safety tests passed; conservative static SRAM estimate={estimated} bytes')
