#!/usr/bin/env python3
from pathlib import Path
import re
html=Path(__file__).resolve().parents[1].joinpath('index.html').read_text(encoding='utf-8')
ids=re.findall(r'\bid="([^"]+)"',html)
dups=sorted({x for x in ids if ids.count(x)>1})
assert not dups, f'Duplicate IDs: {dups}'
required={'profileSelect','physicalConfirm','connectBtn','checkIdBtn','eraseBtn','writeBtn','crcBtn','readBtn','pageSize','writeMode','log'}
assert required.issubset(set(ids)), required-set(ids)
assert 'P${profile.id},${profile.key}' in html
assert 'profile.fast?' in html
assert "fast:false" in html
print('HTML structure tests passed')
