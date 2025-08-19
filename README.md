# EAE_Firmware

Demo firmware-like project that:
- simulates CANBUS send/receive (in-memory)
- implements a PID loop
- includes a state machine for cooling states
- accepts command-line setpoints
- uses CMake to manage build and dependencies (GoogleTest via FetchContent)
- builds on Linux; includes script to build+run
- includes unit tests using GTest

## Quick start (Linux)

Requirements:
- git, cmake >= 3.16, a C++17 compiler (g++/clang++)
- (optional) vcpkg if you prefer to manage deps that way

Build & run:
```bash
# from repo root
chmod +x scripts/build_and_run.sh
./scripts/build_and_run.sh --temp 45 --run-seconds 30
