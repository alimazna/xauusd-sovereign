# XAUUSD Sovereign — Phase 9 Dear ImGui Control Center

Phase 9 is a desktop UI prototype using Dear ImGui, GLFW, OpenGL 3.3+, and C++20.
The archive contains only the UI layer. Dear ImGui, GLFW, and glad are fetched by CMake at configure time.

## Prerequisites

- CMake 3.20+
- C++20 compiler (MSVC, GCC, or Clang)
- OpenGL 3.3+ capable GPU
- The existing Phase 0–8 tree at `src/contracts/foundation/`
- Network access for CMake `FetchContent` on the first configure, unless the dependency sources are already cached by CMake

## Build

From the project root, after extracting this Phase 9 archive:

```bash
cmake -S . -B build
cmake --build build --config Release
```

On multi-config generators, select `Release` in the usual way for the generator.

## Run

```bash
./build/xauusd_ui
```

On Windows, run `build/Release/xauusd_ui.exe` for a Visual Studio multi-config build.

## Integration

`MainLayout::bind(...)` accepts const pointers to the Phase 0–8 foundation stores and engines. Adapters copy data for rendering and never mutate those sources.

The default application starts unbound so the prototype can be launched without a pre-existing runtime wiring layer.

## Known limitations

- Market data is intentionally a placeholder because the Phase 9 specification does not define a MarketAdapter.
- `ValidationFirewall` exposes run counts and per-run result lookup, but not run enumeration; the Validation panel therefore cannot invent a list of validation runs.
- `ApprovalGate` exposes pending counts and decision records, but not pending request enumeration; Approve/Reject controls are UI placeholders and do not mutate the foundation.
- `ScheduleManager` exposes current mode and event history, but not its configured `OperatingSchedule` windows.
- Styling and charting remain intentionally minimal for the prototype phase.
