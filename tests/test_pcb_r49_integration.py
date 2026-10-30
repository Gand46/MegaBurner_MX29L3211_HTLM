#!/usr/bin/env python3
"""Static integration tests across original R4.9 KiCad, CSV and v0.7A.
No physical measurement, AVR binary compilation or electrical sign-off is implied.
"""
from pathlib import Path
import csv, json, re, hashlib
ROOT = Path(__file__).resolve().parent.parent
PCB = ROOT / 'hardware/pcb/R4.9-2L-FINAL'

# Map of the supplied 44-pin source, not reconstructed from legacy v0.5A.
rows = list(csv.DictReader((PCB / 'docs/NETLIST_AND_PINOUT.csv').open(newline='', encoding='utf-8-sig')))
assert len(rows) == 44, 'not exactly 44 SOP44 positions'
pins = {int(r['SOP44/DIP44 pin']): r for r in rows}
assert set(pins) == set(range(1, 45)), 'duplicate/missing SOP44 pins'
for pin, mega, port, net in [(1,'D7','PH4','A21'),(33,'D6','PH3','WE'),(44,'D45','PL4','A20'),
                             (13,'GND','','GND'),(23,'5V','','+3V3'),(32,'GND','','GND'),
                             (38,'A12','PK4','A12'),(12,'D9','PH6','CE'),(14,'D16','PH1','OE')]:
    r=pins[pin]
    assert (r['Arduino Mega pin'],r['ATmega2560 port'],r['PCB net'])==(mega,port,net), (pin,r)
for i in range(8):
    assert pins[11-i]['PCB net']==f'A{i}'
    assert pins[42-i]['PCB net']==f'A{i+8}'
    assert pins[11-i]['ATmega2560 port']==f'PF{i}'
    assert pins[42-i]['ATmega2560 port']==f'PK{i}'
for i in range(8):
    lo=[15,17,19,21,24,26,28,30][i]
    hi=[16,18,20,22,25,27,29,31][i]
    assert pins[lo]['PCB net']==f'Q{i}' and pins[lo]['ATmega2560 port']==f'PC{i}'
    assert pins[hi]['PCB net']==f'Q{i+8}' and pins[hi]['ATmega2560 port']==f'PA{i}'
    assert pins[lo]['Arduino Mega pin']==f'D{37-i}'
    assert pins[hi]['Arduino Mega pin']==f'D{22+i}'
for pin, net, port in [(34,'A16','PL0'),(3,'A17','PL1'),(2,'A18','PL2'),(43,'A19','PL3')]:
    assert pins[pin]['PCB net']==net and pins[pin]['ATmega2560 port']==port

# Read the actual KiCad native PCB footprint J1; compare pad number and net.
pcb = next((PCB/'kicad').glob('*.kicad_pcb')).read_text(encoding='utf-8')
match = re.search(r'\(footprint "MegaBurner:SA638-B006_DIP44_DIRECT_SOLDER_600MIL_HORIZONTAL"(.*?)\n  \)',pcb,re.S)
assert match, 'SA638 DIP44 footprint missing'
pads = re.findall(r'\(pad "(\d+)"[^\n]*?\(net \d+ "([^"\n]+)"\)', match.group(1))
assert len(pads)==44, f'expected 44 J1 pads, found {len(pads)}'
padmap={int(n):net for n,net in pads}
assert set(padmap)==set(pins)
assert all(padmap[n]==r['PCB net'] for n,r in pins.items()), 'KiCad/CSV net mismatch'

# Config defines functions at the 3 variable physical positions; compare against netlist.
profiles=json.loads((ROOT/'config/chips.json').read_text(encoding='utf-8'))
assert len(profiles)==3
assert [(p['id'],p['key'],p['sizeBytes'],p['manufacturerId'],p['deviceId'],p['fastWrite']) for p in profiles]==[
    (1,'3211',4194304,'00C2','00F9',True),
    (2,'6420',8388608,'00C2','22FC',True),
    (3,'160C',2097152,'00C2','22C4',False)]
colnames=['MX29L3211','MX26L6420','MX29LV160CTMC']
for p,c in zip(profiles,colnames):
    for n in (1,33,44):
        assert p[f'pin{n}'].split(' / ')[0]==pins[n][c], (n,c,p[f'pin{n}'])
assert [p['hardwareValidated'] for p in profiles]==[True,True,False]

fw=ROOT/'firmware/MegaBurner_MultiChip_v0_7A_ProfileSafe'
main=(fw/'MegaBurner_MultiChip_v0_7A_ProfileSafe.ino').read_text()
assert '#define SERIAL_BAUD 500000' in main
for key in ['"3211"','"6420"','"160C"']:
    assert key in main
for bit in ('(1 << 1)','(1 << 3)','(1 << 4)','(1 << 6)'):
    assert bit in main
assert 'DDRF = 0xFF;' in main and 'DDRK = 0xFF;' in main
assert 'DDRC = 0x00;' in main and 'DDRA = 0x00;' in main
assert 'PORTL |= (1 << 4)' in main
c6420=(fw/'MX26L6420SocketDevice.cpp').read_text() + (fw/'MX26L6420SocketDevice.h').read_text()
c3211=(fw/'MX29L3211Device.cpp').read_text()
c160=(fw/'MX29LV160CTSocketDevice.h').read_text()
assert 'PORTH' in c6420 and 'PORTH' in c3211
assert 'PORTL = (PORTL & 0xF0) | ((wordAddress >> 16) & 0x0F)' in c160
assert 'PORTL' in c3211 and 'PORTL' in c6420
assert (ROOT/'web/index.html').read_bytes()==(ROOT/'index.html').read_bytes()

# Original PCB contents are byte-identical (per SHA256SUMS); no accidental artwork change.
checks=0
for line in (PCB/'SHA256SUMS.txt').read_text().splitlines():
    m=re.match(r'^([0-9a-f]{64})\s+(.+)$',line)
    if not m: continue
    f=PCB/m.group(2)
    assert f.is_file(),m.group(2)
    assert hashlib.sha256(f.read_bytes()).hexdigest()==m.group(1),str(f)
    checks+=1
assert checks>=30,checks
print(f'R4.9 PCB integration: PASS: {len(rows)} CSV rows = {len(pads)} KiCad J1 pads; 3 profiles; {checks} original PCB hashes')
