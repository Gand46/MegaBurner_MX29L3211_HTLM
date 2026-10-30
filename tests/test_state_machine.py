#!/usr/bin/env python3
from dataclasses import dataclass

PROFILES={1:{'fast':True,'page':{128,256},'size':4194304},2:{'fast':True,'page':{2},'size':8388608},3:{'fast':False,'page':{2},'size':2097152}}

@dataclass
class State:
    profile:int=0
    locked:bool=False
    id_ok:bool=False
    erase_ok:bool=False
    writing:bool=False

    def select(self,p):
        assert not self.locked and not self.writing
        assert p in PROFILES
        self.profile=p;self.locked=self.id_ok=self.erase_ok=self.writing=False
    def check_id(self,ok):
        assert self.profile
        self.id_ok=ok;self.locked=ok;self.erase_ok=False
    def erase(self):
        assert self.locked and self.id_ok and not self.writing
        self.erase_ok=True
    def begin(self):
        assert self.locked and self.id_ok and self.erase_ok and not self.writing
        self.writing=True
    def block(self,fast,page,offset,size):
        assert self.writing and self.erase_ok
        assert page in PROFILES[self.profile]['page']
        assert offset%2==0 and size%2==0 and 0<size<=4096
        assert offset+size<=PROFILES[self.profile]['size']
        if fast: assert PROFILES[self.profile]['fast']
    def end(self):
        assert self.writing
        self.writing=False;self.erase_ok=False

# Valid flows for each profile.
for p in PROFILES:
    s=State();s.select(p);s.check_id(True);s.erase();s.begin()
    s.block(PROFILES[p]['fast'],min(PROFILES[p]['page']),0,4096)
    s.block(False,min(PROFILES[p]['page']),PROFILES[p]['size']-4096,4096)
    s.end()
    assert s.locked and s.id_ok and not s.erase_ok and not s.writing

# Wrong ID blocks destructive operations.
s=State();s.select(1);s.check_id(False)
try:s.erase();raise AssertionError('erase allowed after wrong ID')
except AssertionError:pass

# Profile 3 rejects Fast and page 256.
s=State();s.select(3);s.check_id(True);s.erase();s.begin()
for fast,page in [(True,2),(False,256)]:
    try:s.block(fast,page,0,4096);raise AssertionError('incompatible option accepted')
    except AssertionError:pass

# Profile is locked after ID success.
s=State();s.select(2);s.check_id(True)
try:s.select(1);raise AssertionError('profile changed after lock')
except AssertionError:pass

print('State-machine tests passed')
