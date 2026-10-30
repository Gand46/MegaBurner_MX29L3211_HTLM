# MegaBurner v0.7A + R4.9 — README docs.2 integration audit

Date: 2026-10-30. Scope: English/Spanish root `README.md`, local photo gallery, prior R4.9 documentation, GitHub Desktop overlay. **No changes to firmware source, HTML, original chip configuration, historical baselines, license, VERSION or PCB manufacturing/KiCad files.**

## Automated checks executed

```text
Host C++ compile OK
All multichip logic tests passed
State-machine tests passed
HTML structure tests passed
Memory/safety tests passed; conservative static SRAM estimate=4368 bytes
R4.9 PCB integration: PASS: 44 CSV rows = 44 KiCad J1 pads; 3 profiles; 38 original PCB hashes
ALL TESTS PASSED
```

- README: **514 lines**, 54569 UTF-8 bytes; EN followed by ES, each with sections 1/2/3/4/5/6/6.5/7/8/9.
- Source image archive: **20 pictures**; selected GitHub gallery **12 optimized assets**, with source SHA-256 recorded in `docs/GALLERY_MANIFEST.json`. Images appear **20 times** throughout the bilingual README.
- Local README links **64 checked**, missing **0**.
- Electrical pin table: all **44 J1 contacts**, repeated in both languages, generated from `docs/PCB_R49_WIRING_AND_POWER.md` (which references the R4.9 original CSV).
- Original v0.7A archive: compared **22 code, baseline, config, HTML, LICENSE and VERSION files**, binary-identical **22**.
- `tests/run_all.sh`: all checks passed on the extracted local source, including host C++ Arduino stub and KiCad CSV integration (see above).
- Authentic Arduino AVR toolchain / physical R4.9 assembly / chip tests: **NOT executed**.

## Warnings and remaining limits

1. The Arduino Mega 2560 **must be fully converted and verified for 3.3 V direct I/O and 3.2–3.4 V at its header labelled `5V`**. Do not mate a stock Mega 5 V to this shield. 16 MHz at 3.3 V needs specific validation or design changes.
2. Real socket adapter mapping DIP44→SOP44, power/current/decoupling, assembled PCB R4.9 functional programming and MX29LV160CT operations require physical proof.
3. Commercial reference images and photos supplied by the project owner have no automated permission/license determination; owner should review public redistribution rights.
4. Prior docs.1 manifest is updated to docs.2 and tracks files by SHA-256, excluding self-referential manifests.
5. README updates do **not** constitute a new firmware release: `VERSION` remains v0.7A.

## Restore / recovery

The complete ZIP includes all accumulated source, PCB, historical documentation, and gallery assets. The Overlay ZIP includes only new/modified files relative to the **original supplied v0.7A archive**, to merge into an existing GitHub Desktop checkout. Both preserve `.git` in the target checkout (no `.git` in packages).
