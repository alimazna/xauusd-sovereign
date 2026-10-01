# XAUUSD Sovereign — Test Report

## New tests
- Unit tests: 21 / 21 PASS
- Integration tests: 1 / 1 PASS
- End-to-end tests: 1 / 1 PASS
- New-test total: 23 / 23 PASS

## Existing project tests
The repaired baseline previously passed Phase 2–8, 10 and 11 direct integration executables; Phase 1 was corrected during the repair pass. The final CMake graph registers those tests plus the 23 new tests.

## Determinism
`test_decision_cycle` runs the same synthetic bar sequence twice and verifies identical regime classifications, signal directions, and score/confidence-derived outputs.

## Compilation
All newly added engine `.cpp` files and the modified `RuntimeEngine.cpp` were compile-checked with C++20 and:
`-Wall -Wextra -Wpedantic`.

## CMake
CMake configuration succeeds in offline foundation/test mode with:
```bash
cmake -S . -B build -DXAUUSD_BUILD_UI=OFF
```
The full UI build depends on downloading GLFW, Dear ImGui docking, and glad when those dependencies are not already cached.
