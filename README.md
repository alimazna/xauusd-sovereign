# XAUUSD Sovereign

## Overview
Deterministic, research-first, multi-timeframe XAUUSD shadow system with human-governed research and evolution architecture.

## Phases
- Phase 0: Immutable Foundations (36 files)
- Phase 0.5: Resilience (35 files)
- Phase 1: Deterministic Runtime (~30 files)
- Phase 2: Observation & Outcomes (27 files)
- Phase 3: Self-Learning (32 files)
- Phase 4: Research Plane (30 files)
- Phase 5: Evolution (22 files)
- Phase 6: Validation (31 files)
- Phase 7: Governance (35 files)
- Phase 8: Operating Window (25 files)
- Phase 9: Desktop UI (~56 files)
- Phase 10: Telegram (~17 files)
- Phase 11: Live Validation (~26 files)

Total: ~406 source files.

## Build Requirements
- C++20 compiler (MSVC 2022 / g++ 11+ / clang 14+)
- CMake 3.20+
- OpenGL 3.3+
- Windows, Linux, or macOS

## Build Instructions
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Run
```bash
./build/xauusd_ui
```

## Test
```bash
cd build
ctest --output-on-failure
```

## External Dependencies
- Dear ImGui (docking branch)
- GLFW
- glad
- libcurl (Telegram HTTP delivery)

## Status
- Shadow mode ready
- No live trading in prototype
- MT5 bridge integrated: local TCP server + 9 MQL5 adapters; default execution remains SHADOW


## MT5 Integration

The project now includes the MT5 bridge under `src/mt5/`. The C++ side listens on `127.0.0.1:5555` and exchanges a 24-byte little-endian header plus UTF-8 JSON payloads with the MQL5 Expert Advisors. The bridge validates CRC32, rejects payloads over 1 MiB, tracks adapter heartbeats, and reconnects from the EA side after transport failure.

MQL5 deployment files are in `src/mt5/mql5/`: nine timeframe adapters (`M1` through `MN1`) plus the shared protocol, socket, adapter, and order-execution support. Live broker execution is disabled by default with `InpEnableLiveOrders=false`; order commands are logged and acknowledged in SHADOW mode until explicitly enabled. The bridge also requires `XAUS_MT5_AUTH_TOKEN`/`InpAuthToken` authentication and routes each trading command to one authenticated timeframe adapter.

See `MT5_SETUP_GUIDE.md` and `MT5_PROTOCOL.md` for deployment and wire-format details. The MQL5 Socket* functions are native MetaTrader 5 network APIs; the terminal must explicitly allow the server address in Expert Advisors settings.

## Complete market-intelligence pipeline

The project now includes the MVP market-intelligence path in addition to the existing
foundation, resilience, research, evolution, validation, governance, UI, and Telegram
layers:

`data -> validation -> timeframe state -> features -> structure -> regime -> eligibility -> signal -> score/confidence -> macro/market quality -> risk -> portfolio risk -> shadow fill -> position simulation -> reconciliation -> audit`.

The MVP intentionally keeps probability calibration unproven and uses an in-memory persistence engine.
MT5/MQL5 adapters are included; live broker execution remains explicitly disabled by default for safety.

## Verification

New unit tests cover each added engine/contract family, plus a pipeline integration test and a deterministic
end-to-end replay test.

```bash
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build --output-on-failure
```

### Telegram bots

Telegram delivery is implemented through the Bot API using libcurl. Two Telegram bot integrations are supported (operations and governance). Real bot tokens and chat IDs are never committed to the repository. For local development, copy `config/telegram.env.example` to `config/telegram.local.env` and fill the values. Environment variables override values from the local file.

Keep `config/telegram.local.env` private. It is excluded by `.gitignore`. Run the application from the project root so the default relative config path can be found, or provide the same settings as environment variables.

The automated test suite never performs live Telegram sends; Telegram tests use deterministic test mode.
## GitHub Actions

The repository includes `.github/workflows/ci.yml`. Every push and pull request runs a clean Ubuntu CI job that:

1. Configures CMake with `XAUUSD_BUILD_UI=OFF` (headless CI mode).
2. Builds the C++ project with Ninja.
3. Runs the complete CTest suite.
4. Uploads test/build artifacts when successful.

The CI job does not require Telegram tokens because the automated Telegram tests are deterministic and do not send live messages. Keep real Telegram credentials in GitHub Secrets for any future deployment workflow; never commit them to the repository.

MQL5/MetaTrader compilation still requires a Windows machine with MetaEditor/MetaTrader 5 and is not performed by the Ubuntu CI job.

