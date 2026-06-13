# Benchmark Results

Local test results gathered during development. These depend on Arduino board, USB bridge, cable, browser and PC.

| Stage | Typical result |
|---|---:|
| v0.3 write, page 128 | ~17m44s |
| v0.4B FastWrite | ~3m04s to ~3m51s |
| v0.3 Read/Dump | ~11m31s |
| v0.4A/v0.5 readBlock 16384 | ~1m24s |
| v0.4C FastCRC | ~1m04s |
| 1M baud | Rejected as unstable in local tests |

Final recommended profile: 500000 baud, Fast write, pageSize 256, readBlock 16384.
