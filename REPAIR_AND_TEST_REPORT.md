# XAUUSD Sovereign — Repair & Verification Report

## Final status

- Foundation build: PASS
- Unit/integration/end-to-end suite: **35/35 PASS (100%)**
- Regression tests added for the previously verified defects: PASS
- Critical runtime lifecycle test: PASS
- AddressSanitizer/UndefinedBehaviorSanitizer on repaired regression + runtime tests: PASS
- UI/default build: not fully verified in this offline environment because external GLFW/ImGui/glad dependencies could not be downloaded from GitHub.

## Main repaired areas

1. SL/TP are carried from RiskProposal -> SimulatedFill -> Position and are enforced during position updates.
2. XAUUSD position sizing is driven by SymbolSpec tick/volume semantics through PositionSizer.
3. PnL uses tick_size/tick_value and direction-aware monetary semantics.
4. Portfolio risk reservations have proposal identity and can be released after close or failed execution.
5. Runtime account balance/equity/daily PnL/peak equity are updated after realized closes.
6. Multi-timeframe IDs use timeframe and parent-derived identity chaining.
7. Out-of-order/stale/duplicate market data is rejected or marked instead of overwriting newer state.
8. SL/TP geometry and RiskLimits finite/semantic validation are enforced.
9. Volume reductions recompute risk metadata.
10. Position close time and close reason are persisted.
11. BarFinalizer validates caller timeframe against Bar timeframe.
12. Data validation records real observation timestamps and rejects impossible tick chronology/NaN/Infinity monetary specs.
13. Canary and schedule transitions enforce their pending/window/state constraints.
14. Graceful degradation and resource governance fail closed for unknown/unavailable capabilities/resources.
15. Audit/health/failure/candidate/notification timestamps and IDs are populated instead of remaining zero.
16. Reconciliation verifies signal/proposal/position identity, requested entry, slippage, SL, TP, volume, and monetary specification.
17. The runtime integration test now executes a deterministic open -> close -> settlement -> reconciliation cycle and verifies that portfolio reservations are released and a second trade can open.

## Test command

```bash
cmake -S . -B build_repaired -DXAUUSD_BUILD_UI=OFF -DCMAKE_BUILD_TYPE=Debug
cmake --build build_repaired -j2
ctest --test-dir build_repaired --output-on-failure --timeout 60
```

## Sanitizer smoke tests

```bash
ASAN_OPTIONS=detect_leaks=1 UBSAN_OPTIONS=print_stacktrace=1 ./build_sanitize/test_regressions
ASAN_OPTIONS=detect_leaks=1 UBSAN_OPTIONS=print_stacktrace=1 ./build_sanitize/test_runtime_lifecycle
```

Both passed.

## Important scope note

This repair makes the shadow/simulation and financial state pipeline internally consistent for the covered behavior. It does **not** activate live broker execution: the project's MT5 bridge remains deferred/inert by design.
