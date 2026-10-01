#include "Timeframe.h"
#include "Bar.h"
#include "Tick.h"
#include "SymbolSpec.h"
#include "ShadowDecision.h"
#include "DataValidator.h"
#include "TimeframeStateStore.h"
#include "BarFinalizer.h"
#include "DataBus.h"
#include "MockDataAdapter.h"
#include "ShadowLedger.h"
#include "RuntimeEngine.h"

#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using namespace xauusd::sovereign;

static int g_failures = 0;

#define CHECK(expr) do {                                              \
    if (!(expr)) {                                                    \
        std::cerr << "FAIL: " #expr                                   \
                  << " at line " << __LINE__ << "\n";                 \
        ++g_failures;                                                 \
    }                                                                 \
} while (0)

static void test_1_timeframe_enum_sanity()
{
    CHECK(static_cast<std::uint8_t>(Timeframe::M1)  == 0);
    CHECK(static_cast<std::uint8_t>(Timeframe::MN1) == 8);
}

static void test_2_bar_ohlc_invariants()
{
    Bar b{};
    b.open = 100.0;
    b.high = 105.0;
    b.low  = 99.0;
    b.close = 104.0;
    b.open_time = Timestamp{1000};
    b.close_time = Timestamp{2000};
    b.timeframe = Timeframe::M15;

    CHECK(b.high >= b.low);
    CHECK(b.high >= b.open);
    CHECK(b.high >= b.close);
    CHECK(b.low  <= b.open);
    CHECK(b.low  <= b.close);
}

static void test_3_validator_accepts_valid_bar()
{
    DataValidator validator;

    Bar good{};
    good.open = 100.0;
    good.high = 105.0;
    good.low  = 99.0;
    good.close = 104.0;
    good.open_time = Timestamp{1000};
    good.close_time = Timestamp{2000};
    good.timeframe = Timeframe::M15;

    auto r = validator.validate_bar(good);
    CHECK(r.outcome == ValidationOutcome::ACCEPTED);
    CHECK(r.quality == DataQualityState::VALID);
}

static void test_4_validator_rejects_invalid_ohlc()
{
    DataValidator validator;

    Bar bad{};
    bad.open = 100.0;
    bad.high = 99.0;
    bad.low  = 101.0;
    bad.close = 104.0;
    bad.open_time = Timestamp{1000};
    bad.close_time = Timestamp{2000};
    bad.timeframe = Timeframe::M15;

    auto r = validator.validate_bar(bad);
    CHECK(r.outcome == ValidationOutcome::REJECTED);
    CHECK(r.quality == DataQualityState::INVALID);
}

static void test_5_validator_accepts_valid_tick()
{
    DataValidator validator;

    Tick t{};
    t.bid = 2000.50;
    t.ask = 2000.60;
    t.last = 2000.55;
    t.volume = 1.0;
    t.event_time = Timestamp{1000};
    t.receive_time = Timestamp{1010};

    auto r = validator.validate_tick(t);
    CHECK(r.outcome == ValidationOutcome::ACCEPTED);
}

static void test_6_validator_rejects_ask_below_bid()
{
    DataValidator validator;

    Tick t{};
    t.bid = 2000.60;
    t.ask = 2000.50;
    t.last = 2000.55;
    t.volume = 1.0;

    auto r = validator.validate_tick(t);
    CHECK(r.outcome == ValidationOutcome::REJECTED);
}

static void test_7_validator_accepts_valid_symbol_spec()
{
    DataValidator validator;

    SymbolSpec spec{};
    spec.broker = "TestBroker";
    spec.server = "TestServer";
    spec.symbol = "XAUUSD";
    spec.digits = 2;
    spec.point = 0.01;
    spec.tick_size = 0.01;
    spec.tick_value = 1.0;
    spec.contract_size = 100.0;
    spec.volume_min = 0.01;
    spec.volume_max = 100.0;
    spec.volume_step = 0.01;
    spec.stops_level = 0.0;
    spec.freeze_level = 0.0;

    auto r = validator.validate_symbol_spec(spec);
    CHECK(r.outcome == ValidationOutcome::ACCEPTED);
}

static void test_8_validator_rejects_empty_symbol()
{
    DataValidator validator;

    SymbolSpec spec{};
    spec.symbol = "";
    spec.digits = 2;
    spec.point = 0.01;
    spec.tick_size = 0.01;
    spec.contract_size = 100.0;
    spec.volume_min = 0.01;
    spec.volume_max = 100.0;
    spec.volume_step = 0.01;

    auto r = validator.validate_symbol_spec(spec);
    CHECK(r.outcome == ValidationOutcome::REJECTED);
}

static void test_9_timeframe_state_store()
{
    TimeframeStateStore store;

    CHECK(store.size() == 0);
    CHECK(!store.has_state(Timeframe::M15));

    TimeframeState st{};
    st.timeframe = Timeframe::M15;
    st.quality = DataQualityState::VALID;
    st.finalization = BarFinalizationState::FINALIZED;

    store.put_state(st);

    CHECK(store.size() == 1);
    CHECK(store.has_state(Timeframe::M15));

    TimeframeState out{};
    CHECK(store.get_state(Timeframe::M15, out));
    CHECK(out.timeframe == Timeframe::M15);

    store.clear();
    CHECK(store.size() == 0);
}

static void test_10_data_bus()
{
    DataBus bus;

    int tick_count = 0;
    int bar_count  = 0;

    bus.subscribe_ticks([&](const Tick&) { ++tick_count; });
    bus.subscribe_bars([&](const Bar&)  { ++bar_count;  });

    CHECK(bus.tick_subscriber_count() == 1);
    CHECK(bus.bar_subscriber_count()  == 1);

    Tick t{};
    bus.publish_tick(t);
    bus.publish_tick(t);

    CHECK(tick_count == 2);

    Bar b{};
    bus.publish_bar(b);

    CHECK(bar_count == 1);

    bus.subscribe_ticks([&](const Tick&) { ++tick_count; });
    bus.publish_tick(t);

    CHECK(tick_count == 4);
}

static void test_11_bar_finalizer_finalizes_valid_bar()
{
    DataValidator validator;
    BarFinalizer finalizer(&validator);

    Bar good{};
    good.open = 100.0;
    good.high = 105.0;
    good.low  = 99.0;
    good.close = 104.0;
    good.open_time = Timestamp{1000};
    good.close_time = Timestamp{2000};
    good.timeframe = Timeframe::M15;

    DataValidationResult vr{};
    auto fs = finalizer.process_bar(Timeframe::M15, good, vr);

    CHECK(fs == BarFinalizationState::FINALIZED);
    CHECK(vr.outcome == ValidationOutcome::ACCEPTED);
}

static void test_12_bar_finalizer_rejects_invalid_bar()
{
    DataValidator validator;
    BarFinalizer finalizer(&validator);

    Bar bad{};
    bad.open = 100.0;
    bad.high = 99.0;
    bad.low  = 101.0;
    bad.close = 104.0;
    bad.open_time = Timestamp{1000};
    bad.close_time = Timestamp{2000};
    bad.timeframe = Timeframe::M15;

    DataValidationResult vr{};
    auto fs = finalizer.process_bar(Timeframe::M15, bad, vr);

    CHECK(fs == BarFinalizationState::REJECTED);
    CHECK(vr.outcome == ValidationOutcome::REJECTED);
}

static void test_13_bar_finalizer_null_validator()
{
    BarFinalizer finalizer(nullptr);

    Bar any{};
    DataValidationResult vr{};
    auto fs = finalizer.process_bar(Timeframe::M15, any, vr);

    CHECK(fs == BarFinalizationState::UNKNOWN);
}

static void test_14_mock_data_adapter()
{
    MockDataAdapter adapter;

    CHECK(!adapter.is_connected());

    SymbolSpec spec{};
    CHECK(!adapter.fetch_symbol_spec(spec));

    adapter.set_connected(true);
    CHECK(adapter.is_connected());

    SymbolSpec spec2{};
    spec2.symbol = "XAUUSD";
    spec2.digits = 2;
    spec2.point = 0.01;
    spec2.tick_size = 0.01;
    spec2.contract_size = 100.0;
    spec2.volume_min = 0.01;
    spec2.volume_max = 100.0;
    spec2.volume_step = 0.01;

    adapter.set_symbol_spec(spec2);
    CHECK(adapter.fetch_symbol_spec(spec));
    CHECK(spec.symbol == "XAUUSD");
}

static void test_15_mock_data_adapter_bars()
{
    MockDataAdapter adapter;

    std::vector<Bar> bars;
    for (int i = 0; i < 5; ++i) {
        Bar b{};
        b.open_time = Timestamp{static_cast<std::int64_t>(i * 1000)};
        b.close_time = Timestamp{static_cast<std::int64_t>((i + 1) * 1000)};
        b.open = 100.0 + i;
        b.high = b.open + 1.0;
        b.low  = b.open - 1.0;
        b.close = b.open + 0.5;
        b.timeframe = Timeframe::M15;
        bars.push_back(b);
    }

    adapter.set_recent_bars(Timeframe::M15, bars);

    std::vector<Bar> out;
    CHECK(adapter.fetch_recent_bars(Timeframe::M15, 3, out));
    CHECK(out.size() == 3);
    CHECK(out.front().open == 102.0);
    CHECK(out.back().open  == 104.0);
}

static void test_16_shadow_ledger_append_and_duplicate_rejection()
{
    ShadowLedger ledger;

    CHECK(ledger.size() == 0);

    ShadowLedgerEntry e1{};
    std::array<std::uint8_t, 16> bytes1{};
    bytes1[0] = 1;
    e1.entry_id = EntityId{bytes1};

    CHECK(ledger.append(e1));
    CHECK(ledger.size() == 1);
    CHECK(ledger.contains(e1.entry_id));

    CHECK(!ledger.append(e1));
    CHECK(ledger.size() == 1);

    ShadowLedgerEntry e2{};
    std::array<std::uint8_t, 16> bytes2{};
    bytes2[0] = 2;
    e2.entry_id = EntityId{bytes2};

    CHECK(ledger.append(e2));
    CHECK(ledger.size() == 2);
}

static void test_17_shadow_ledger_get()
{
    ShadowLedger ledger;

    ShadowLedgerEntry e1{};
    std::array<std::uint8_t, 16> bytes1{};
    bytes1[0] = 42;
    e1.entry_id = EntityId{bytes1};
    e1.notes = "hello";

    ledger.append(e1);

    ShadowLedgerEntry out{};
    CHECK(ledger.get(e1.entry_id, out));
    CHECK(out.notes == "hello");
}

static void test_18_runtime_engine_ingest_bar()
{
    DataValidator validator;
    TimeframeStateStore store;
    ShadowLedger ledger;

    BarFinalizer finalizer(&validator);
    RuntimeEngine engine(&finalizer, &store, &ledger);

    Bar good{};
    good.open = 100.0;
    good.high = 105.0;
    good.low  = 99.0;
    good.close = 104.0;
    good.open_time = Timestamp{1000};
    good.close_time = Timestamp{2000};
    good.timeframe = Timeframe::M15;

    auto fs = engine.ingest_bar(Timeframe::M15, good);

    CHECK(fs == BarFinalizationState::FINALIZED);
    CHECK(store.has_state(Timeframe::M15));

    auto snap = engine.snapshot();
    CHECK(snap.bars_processed == 1);
    CHECK(snap.decisions_recorded == 0);
}

static void test_19_runtime_engine_record_decision()
{
    DataValidator validator;
    TimeframeStateStore store;
    ShadowLedger ledger;
    BarFinalizer finalizer(&validator);
    RuntimeEngine engine(&finalizer, &store, &ledger);

    ShadowDecision d{};
    std::array<std::uint8_t, 16> bytes{};
    bytes[0] = 7;
    d.decision_id = EntityId{bytes};
    d.trigger_timeframe = Timeframe::M15;

    CHECK(engine.record_decision(d));
    CHECK(!engine.record_decision(d));

    auto snap = engine.snapshot();
    CHECK(snap.decisions_recorded == 1);
}

static void test_20_end_to_end_flow()
{
    DataValidator validator;
    TimeframeStateStore store;
    ShadowLedger ledger;
    BarFinalizer finalizer(&validator);
    RuntimeEngine engine(&finalizer, &store, &ledger);

    for (int i = 0; i < 3; ++i) {
        Bar b{};
        b.open_time = Timestamp{static_cast<std::int64_t>(i * 1000)};
        b.close_time = Timestamp{static_cast<std::int64_t>((i + 1) * 1000)};
        b.open = 100.0 + i;
        b.high = b.open + 1.0;
        b.low  = b.open - 1.0;
        b.close = b.open + 0.5;
        b.timeframe = Timeframe::M15;

        CHECK(engine.ingest_bar(Timeframe::M15, b)
              == BarFinalizationState::FINALIZED);
    }

    for (std::uint8_t i = 0; i < 2; ++i) {
        ShadowDecision d{};
        std::array<std::uint8_t, 16> id_bytes{};
        id_bytes[0] = i + 100;
        d.decision_id = EntityId{id_bytes};
        d.trigger_timeframe = Timeframe::M15;
        CHECK(engine.record_decision(d));
    }

    auto snap = engine.snapshot();
    CHECK(snap.bars_processed == 3);
    CHECK(snap.decisions_recorded == 2);
    CHECK(ledger.size() == 2);
}

int main()
{
    test_1_timeframe_enum_sanity();
    test_2_bar_ohlc_invariants();
    test_3_validator_accepts_valid_bar();
    test_4_validator_rejects_invalid_ohlc();
    test_5_validator_accepts_valid_tick();
    test_6_validator_rejects_ask_below_bid();
    test_7_validator_accepts_valid_symbol_spec();
    test_8_validator_rejects_empty_symbol();
    test_9_timeframe_state_store();
    test_10_data_bus();
    test_11_bar_finalizer_finalizes_valid_bar();
    test_12_bar_finalizer_rejects_invalid_bar();
    test_13_bar_finalizer_null_validator();
    test_14_mock_data_adapter();
    test_15_mock_data_adapter_bars();
    test_16_shadow_ledger_append_and_duplicate_rejection();
    test_17_shadow_ledger_get();
    test_18_runtime_engine_ingest_bar();
    test_19_runtime_engine_record_decision();
    test_20_end_to_end_flow();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    }

    std::cerr << g_failures << " TEST(S) FAILED\n";
    return 1;
}
