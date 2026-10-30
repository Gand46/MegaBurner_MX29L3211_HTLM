#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
FW="$ROOT/firmware/MegaBurner_MultiChip_v0_7A_ProfileSafe"
STUB="$ROOT/tests/arduino_stub"
BUILD="$ROOT/tests/build"
rm -rf "$BUILD" && mkdir -p "$BUILD"
cp "$FW/MegaBurner_MultiChip_v0_7A_ProfileSafe.ino" "$BUILD/main.cpp"
for f in "$FW"/*.cpp "$BUILD/main.cpp" "$STUB/Arduino.cpp"; do
  g++ -std=c++17 -Wall -Wextra -Wconversion -Wshadow -I"$STUB" -I"$FW" -c "$f" -o "$BUILD/$(basename "$f").o"
done
echo "Host C++ compile OK"
