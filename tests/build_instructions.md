# Build Instructions

## Prerequisites
- C++20 compiler
- CMake 3.20+
- Git
- OpenGL development package for the Desktop UI

## Build
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

## Tests
```bash
ctest --test-dir build --output-on-failure
```

## UI
Linux/macOS:
```bash
./build/xauusd_ui
```

Windows:
```powershell
.\build\Release\xauusd_ui.exe
```

Dear ImGui is pulled from the docking tag, matching the docking APIs used by the UI.
MT5/MQL5 adapters are intentionally not included in this MVP because they require a real
MT5 environment; `MockDataAdapter` is used for deterministic local testing.
