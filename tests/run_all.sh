#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"
./tests/host_compile.sh
python3 tests/test_logic.py
python3 tests/test_state_machine.py
python3 tests/test_html_structure.py
python3 tests/test_memory_and_safety.py
node --check tests/ui.js
echo "ALL TESTS PASSED"
