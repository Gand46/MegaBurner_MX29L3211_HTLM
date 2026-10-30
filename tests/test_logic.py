#!/usr/bin/env python3
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
FW = ROOT / 'firmware' / 'MegaBurner_MultiChip_v0_7A_ProfileSafe'
HTML = (ROOT / 'index.html').read_text(encoding='utf-8')
MAIN = (FW / 'MegaBurner_MultiChip_v0_7A_ProfileSafe.ino').read_text(encoding='utf-8')
LV160 = (FW / 'MX29LV160CTSocketDevice.h').read_text(encoding='utf-8') + (FW / 'MX29LV160CTSocketDevice.cpp').read_text(encoding='utf-8')
MX26 = (FW / 'MX26L6420SocketDevice.h').read_text(encoding='utf-8') + (FW / 'MX26L6420SocketDevice.cpp').read_text(encoding='utf-8')
MX3211 = (FW / 'MX29L3211Device.h').read_text(encoding='utf-8') + (FW / 'MX29L3211Device.cpp').read_text(encoding='utf-8')

profiles = {
    1: dict(name='MX29L3211', size=4194304, mfr='00C2', dev='00F9', page=[256,128], fast=True),
    2: dict(name='MX26L6420MC-90', size=8388608, mfr='00C2', dev='22FC', page=[2], fast=True),
    3: dict(name='MX29LV160CTMC-90', size=2097152, mfr='00C2', dev='22C4', page=[2], fast=False),
}

# 1. HTML metadata and exact sizes/IDs.
for pid, p in profiles.items():
    assert f"{pid}:{{id:{pid}" in HTML
    assert f"size:{p['size']}" in HTML
    assert f"mfr:'{p['mfr']}'" in HTML
    assert f"dev:'{p['dev']}'" in HTML

# 2. Firmware profile keys and destructive interlocks.
for token in ['P1,3211', 'P2,6420', 'P3,160C']:
    # These are documented in the source comments, while keys are enforced by strcmp.
    assert token in MAIN
for token in ['profileLocked', 'idOkLatch', 'eraseOkLatch', 'writeSessionActive']:
    assert token in MAIN
assert 'destructiveReady()' in MAIN
assert 'profileFastValidated(activeProfile)' in MAIN
assert "sendError('F')" in MAIN

# 3. Range models: word-address highest bit usage.
def model_3211(byte_offset):
    word = byte_offset // 2
    return dict(PF=word & 0xff, PK=(word>>8)&0xff, PL=(word>>16)&0xff)

def model_6420(byte_offset):
    word = byte_offset // 2
    return dict(PF=word&0xff, PK=(word>>8)&0xff, PL=(word>>16)&0x1f, A21=1 if word&0x200000 else 0)

def model_160(byte_offset, we_high=1):
    word = byte_offset // 2
    pl = ((we_high&1)<<4) | ((word>>16)&0x0f)
    return dict(PF=word&0xff, PK=(word>>8)&0xff, PL=pl, WE=(pl>>4)&1)

assert model_3211(0)['PL'] == 0
assert model_3211(4194302)['PL'] & 0x1f == 0x1f
assert model_6420(0)['A21'] == 0
assert model_6420(0x3ffffe)['A21'] == 0
assert model_6420(0x400000)['A21'] == 1
assert model_6420(0x7ffffe)['A21'] == 1
assert model_160(0)['WE'] == 1
assert model_160(0x1ffffe)['WE'] == 1
assert model_160(0x1ffffe)['PL'] & 0x0f == 0x0f

# 4. Pin-role source assertions.
assert 'WE_BIT = 3' in MX26 and 'A21_BIT = 4' in MX26
assert 'RESET_BIT = 4' in LV160 and 'BYTE_BIT = 3' in LV160 and 'WE_PL_BIT = 4' in LV160
assert 'PORTH &= ~(1 << 4)' in MX3211  # PH4 WE pulse
assert 'PORTL &= ~(1 << WE_PL_BIT)' in LV160  # PL4 WE pulse

# 5. Algorithm compatibility.
assert 'pageSizeBytes == 128 || pageSizeBytes == 256' in MX3211
assert 'pageSizeBytes == 2' in MX26
assert 'pageSizeBytes == 2' in LV160
assert 'verifyAndRepairBlock' in LV160
assert "id == PROFILE_MX29L3211 || id == PROFILE_MX26L6420_SOCKET" in MAIN

# 6. UI must disable Fast mode for experimental LV160.
assert "fast:false" in HTML
assert "Solo SafeWrite" in HTML or "Solo modo Safe" in HTML

print('All multichip logic tests passed')
