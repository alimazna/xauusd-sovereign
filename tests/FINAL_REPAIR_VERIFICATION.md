# XAUUSD Sovereign + MT5 — Final Repair Verification

Date: 28 September 2026

## Scope

This release applies the MT5 audit findings against the source tree, adds regression coverage for the repaired integration paths, rebuilds the project, runs the full CTest suite, and repeats the suite three times to check stability.

## Repairs applied

1. Added an explicit C++ live-command boundary for NEW/CLOSE/MODIFY/CANCEL and live decision submission.
2. Added client/timeframe-aware routing so trading commands are never broadcast to every EA.
3. Added authenticated handshake using a configured token, plus protocol-version and timeframe validation.
4. Added per-client receive sequence monotonicity/replay rejection.
5. Switched MT5 tick timestamps to `MqlTick.time_msc`.
6. Reworked filling-mode selection to inspect symbol execution/filling flags and reject unsupported Market Execution configurations instead of falling back to RETURN.
7. Added strict BUY/SELL validation and numeric input validation.
8. Added `OrderCheck()` before live `OrderSend()` for NEW/CLOSE/MODIFY/CANCEL.
9. Added trade-transaction correlation using the command ID embedded in the request comment and transaction order/deal/position identities.
10. Added C++-side symbol/timeframe validation and session symbol binding.
11. Updated connection freshness tracking to use any authenticated protocol activity, not heartbeat alone.
12. Updated protocol documentation and regression tests for the new routing/auth/transaction contract.

## Verification results

### C++ build

`cmake -S . -B build -DXAUUSD_BUILD_UI=OFF`

Result: **PASS**

`cmake --build build -j2`

Result: **PASS**

### Full CTest

`ctest --test-dir build --output-on-failure`

Result: **39/39 PASS, 0 FAIL**

### Stability repeat

`ctest --test-dir build --output-on-failure --repeat until-fail:3`

Result: **all 39 tests passed on 3 consecutive runs; 0 failures**

### MT5/MQL5 static validation

`python3 tests/mt5/test_mql5_static.py`

Result: **10/10 checks PASS**

Coverage includes authentication, millisecond tick timestamp, timeframe routing, strict side validation, execution/filling inspection, Market Execution RETURN protection, OrderCheck, trade-transaction correlation, and all nine EA transaction forwarding hooks.

### Dedicated MT5 integration coverage

`test_mt5_integration` verifies:

- wrong-token handshake rejection;
- duplicate timeframe route rejection;
- successful M15/M30 authenticated registration;
- NEW_ORDER routed only to M15;
- CLOSE_ORDER routed with timeframe;
- MODIFY_ORDER routed with timeframe;
- CANCEL_ORDER routed with timeframe;
- M30 does not receive M15 trading commands.

## Remaining environment limitation

The Linux audit environment does not provide MetaEditor/MetaTrader 5, so `.mq5` files were validated by static checks but were not compiled/executed inside a real MT5 terminal in this environment.

Before enabling live orders on a real account, compile all nine EAs in MetaEditor on Windows and perform a demo-account end-to-end test covering request acceptance, `OnTradeTransaction`, broker execution, and reconciliation.

## Release status

The repaired source tree is internally buildable and passes the complete automated regression suite. The release is suitable for the next MT5/Windows validation stage; it should not be represented as broker-live validated until that external MT5 test is completed.
