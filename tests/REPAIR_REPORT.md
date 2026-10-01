# XAUUSD Sovereign — Repair Report

## Scope
The original archive contained 406 files. This repair preserves all original project files and adds four requested delivery files:
- `src/contracts/foundation/DataValidationResult.h`
- `README.md`
- `REPAIR_REPORT.md`
- `build_instructions.md`

No original files were deleted.

## Contract Drift Fixed
### `ValidationResult` split
Phase 1 data validation and Phase 6 model validation were using the same type name with incompatible meanings.

Phase 1 now uses `DataValidationResult` with:
- `outcome`
- `quality`
- `reason`
- `observed_at`

Phase 6 keeps the existing `ValidationResult` contract with:
- `result_id`
- `run_id`
- `method`
- `passed`
- `score`
- `threshold`
- `trials`
- `summary`
- `recorded_at`

Affected Phase 1 contracts/tests were updated accordingly.

## Blockers Fixed
1. `src/contracts/foundation/DataValidator.cpp` — replaced plain-text instructions with a complete deterministic implementation for bar, tick, and symbol-spec validation.
2. `src/contracts/foundation/DataValidator.h` — switched return types to `DataValidationResult`.
3. `src/contracts/foundation/IDataValidator.h` — switched Phase 1 validator methods to `DataValidationResult`.
4. `src/contracts/foundation/MockDataAdapter.cpp` — replaced checklist text with a complete in-memory adapter implementation.
5. `src/contracts/foundation/MockDataAdapter.h` — added self-contained standard-library includes.
6. `src/contracts/foundation/BarFinalizer.cpp` — added self-include, namespace, and `process_bar` implementation.
7. `src/contracts/foundation/BarFinalizer.h` — switched output contract to `DataValidationResult`.
8. `src/contracts/foundation/IBarFinalizer.h` — switched output contract to `DataValidationResult`.
9. `src/contracts/foundation/DataBus.cpp` — added self-include and restored the namespace around definitions.
10. `src/contracts/foundation/TimeframeStateStore.cpp` — added self-include and restored the namespace around definitions.
11. `src/contracts/foundation/RuntimeEngine.cpp` — changed ingest validation to `DataValidationResult` and guarded the ledger accessor.
12. `src/contracts/foundation/Phase1IntegrationTests.cpp` — updated the Phase 1 result variables to the new data-validation contract.

## Runtime Bugs Fixed
13. `src/contracts/foundation/ResourceBudget.h` — initialized numeric budget fields to zero.
14. `src/contracts/foundation/ResourceUsage.h` — initialized numeric usage fields to zero.
15. `src/contracts/foundation/TelegramGateway.cpp` — `send()` now honors `available_` and records an unavailable-gateway failure.
16. `src/contracts/foundation/ValidationRunner.h` — added a result-ID salt parameter.
17. `src/contracts/foundation/ValidationRunner.cpp` — mixes the walk-forward window index into the generated result ID.
18. `src/contracts/foundation/CandidateComparator.cpp` — replaced XOR-based comparison IDs with a deterministic FNV-based hash.
19. `src/contracts/foundation/SystemSupervisor.h` — made mutating supervisor dependencies non-const and `evaluate()` non-const.
20. `src/contracts/foundation/SystemSupervisor.cpp` — removed `const_cast` and performs direct stateful calls.
21. `src/contracts/foundation/SubsystemIsolationManager.h` — made `isolate()` non-const and removed `mutable` storage.
22. `src/contracts/foundation/SubsystemIsolationManager.cpp` — updated implementation for the non-const isolation API.
23. `src/contracts/foundation/ResilienceOrchestrator.h` — updated `evaluate_failure()` to match the stateful supervisor API.
24. `src/contracts/foundation/ResilienceOrchestrator.cpp` — updated the implementation to match the new signature.

## UI Fixes
25. `src/ui/adapters/ObservationAdapter.h` / `.cpp` — added explicit per-source status for predictions, outcomes, and failures.
26. `src/ui/adapters/GovernanceAdapter.h` / `.cpp` — added explicit per-source status for approvals and incidents.
27. `src/ui/adapters/LearningAdapter.h` / `.cpp` — added explicit per-source status for knowledge and failure memory.
28. `src/ui/adapters/ResearchAdapter.h` / `.cpp` — added explicit per-source status for hypotheses and experiments.
29. `src/ui/adapters/ScheduleAdapter.h` / `.cpp` — added explicit per-source status for schedule and checkpoints.
30. `src/ui/panels/DashboardPanel.cpp` — dashboard rows now report the state of the exact backing source rather than an aggregate OR state.

## Build Fixes
31. `CMakeLists.txt` — switched Dear ImGui from the non-docking tag to `v1.91.9b-docking`, matching the docking APIs used by the UI.
32. `CMakeLists.txt` — added CTest registration for Phase 1–8, 10, and 11 integration tests.

## Documentation Added
33. `README.md` — added the requested project overview, phase map, requirements, build/run/test commands, dependencies, and current status.
34. `build_instructions.md` — added platform-specific prerequisites and build/test instructions.
35. `REPAIR_REPORT.md` — this repair and verification record.
36. `src/contracts/foundation/DataValidationResult.h` — new Phase 1 data-validation contract.

## Verification Performed
### Individual translation-unit checks
All six explicitly requested repaired files compile with C++20 and warnings enabled:
- `DataValidator.cpp`
- `MockDataAdapter.cpp`
- `BarFinalizer.cpp`
- `DataBus.cpp`
- `TimeframeStateStore.cpp`
- `RuntimeEngine.cpp`

Additionally, all 65 non-test foundation translation units compiled successfully with C++20.

### CMake verification
The repaired CMake project was configured successfully and the following targets were built successfully in an offline verification environment with API-compatible local dependency stubs:
- `xauusd_foundation`
- `xauusd_ui`
- all registered Phase test executables

The UI sources therefore passed C++ compilation and linking through the repaired CMake target graph.

### Integration tests
CTest executed 10 registered integration tests and reported:

`100% tests passed, 0 tests failed out of 10`

Passed:
- Phase1IntegrationTests
- Phase2IntegrationTests
- Phase3IntegrationTests
- Phase4IntegrationTests
- Phase5IntegrationTests
- Phase6IntegrationTests
- Phase7IntegrationTests
- Phase8IntegrationTests
- Phase10IntegrationTests
- Phase11IntegrationTests

The standalone `IntegrationTests` and `tests/test_foundation_001.cpp` checks also pass.

### UI smoke flow
`xauusd_ui` was linked successfully and its initialize/run/shutdown flow was smoke-tested against the offline API stubs; the one-frame smoke execution completed without a program-level failure.

## Environment Limitation
The environment used for this repair could not resolve `github.com`, so the real third-party GLFW/ImGui/glad downloads were not performed here. The final `CMakeLists.txt` retains the intended external dependencies; on a normal network-enabled machine, CMake will fetch them. The Dear ImGui choice is specifically the docking variant because the UI uses `ImGuiConfigFlags_DockingEnable` and `ImGui::DockSpaceOverViewport`, which are documented docking APIs.

## Final State
The repaired project contains the original 406 project files plus the four requested delivery files, for 410 files total before ZIP packaging.

Expected build flow:

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
ctest --output-on-failure
```
