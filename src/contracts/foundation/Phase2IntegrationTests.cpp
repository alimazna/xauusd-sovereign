#include "PredictionLedger.h"
#include "OutcomeEngine.h"
#include "FailureDetector.h"
#include "HealthAggregator.h"
#include "ObservationOrchestrator.h"
#include "Prediction.h"
#include "PredictionRecord.h"
#include "Outcome.h"
#include "OutcomeRecord.h"
#include "FailurePattern.h"
#include "HealthSnapshot.h"

#include <array>
#include <cstdint>
#include <iostream>

using namespace xauusd::sovereign;

static int g_failures = 0;

#define CHECK(expr) do {                                              \
    if (!(expr)) {                                                    \
        std::cerr << "FAIL: " #expr                                   \
                  << " at line " << __LINE__ << "\n";                 \
        ++g_failures;                                                 \
    }                                                                 \
} while (0)

static EntityId make_id(std::uint8_t seed) {
    std::array<std::uint8_t, 16> bytes{};
    bytes[0] = seed;
    return EntityId{bytes};
}

int main() {
    // Test 1 — PredictionLedger append + duplicate rejection
    {
        PredictionLedger ledger;

        CHECK(ledger.size() == 0);
        CHECK(!ledger.contains(make_id(1)));

        PredictionRecord r{};
        r.record_id = make_id(1);

        CHECK(ledger.append(r));
        CHECK(ledger.size() == 1);
        CHECK(ledger.contains(make_id(1)));
        CHECK(!ledger.append(r));
        CHECK(ledger.size() == 1);
    }

    // Test 2 — PredictionLedger get + all
    {
        PredictionLedger ledger;

        PredictionRecord r1{};
        r1.record_id = make_id(10);
        r1.notes = "first";

        PredictionRecord r2{};
        r2.record_id = make_id(20);
        r2.notes = "second";

        ledger.append(r1);
        ledger.append(r2);

        PredictionRecord out{};
        CHECK(ledger.get(make_id(10), out));
        CHECK(out.notes == "first");

        auto all = ledger.all();
        CHECK(all.size() == 2);
        CHECK(all[0].notes == "first");
        CHECK(all[1].notes == "second");
    }

    // Test 3 — OutcomeEngine append + duplicate rejection
    {
        OutcomeEngine engine;

        CHECK(engine.size() == 0);

        OutcomeRecord r{};
        r.record_id = make_id(1);

        CHECK(engine.record(r));
        CHECK(!engine.record(r));
        CHECK(engine.size() == 1);
        CHECK(engine.contains(make_id(1)));
    }

    // Test 4 — OutcomeEngine get
    {
        OutcomeEngine engine;

        OutcomeRecord r{};
        r.record_id = make_id(5);
        r.notes = "outcome";

        engine.record(r);

        OutcomeRecord out{};
        CHECK(engine.get(make_id(5), out));
        CHECK(out.notes == "outcome");

        CHECK(!engine.get(make_id(99), out));
    }

    // Test 5 — FailureDetector observe (new pattern)
    {
        FailureDetector detector;

        CHECK(detector.pattern_count() == 0);

        FailurePattern p{};
        p.pattern_id = make_id(1);
        p.failure_type = FailureType::DATA_FAILURE;
        p.occurrence_count = 1;

        CHECK(detector.observe(p));
        CHECK(detector.pattern_count() == 1);
    }

    // Test 6 — FailureDetector repeated observation updates
    {
        FailureDetector detector;

        FailurePattern p{};
        p.pattern_id = make_id(1);
        p.failure_type = FailureType::DATA_FAILURE;
        p.occurrence_count = 1;
        p.last_seen = Timestamp{100};

        detector.observe(p);

        FailurePattern p2{};
        p2.pattern_id = make_id(1);
        p2.failure_type = FailureType::DATA_FAILURE;
        p2.occurrence_count = 2;
        p2.last_seen = Timestamp{200};

        CHECK(detector.observe(p2));
        CHECK(detector.pattern_count() == 1);

        auto all = detector.all_patterns();
        CHECK(all.size() == 1);
        CHECK(all[0].occurrence_count == 2);
        CHECK(all[0].last_seen.value() == 200);
    }

    // Test 7 — FailureDetector generate_report
    {
        FailureDetector detector;

        FailurePattern p{};
        p.pattern_id = make_id(1);
        p.failure_type = FailureType::DATA_FAILURE;
        detector.observe(p);

        auto report = detector.generate_report();
        CHECK(report.patterns.size() == 1);
    }

    // Test 8 — HealthAggregator observe_snapshot
    {
        HealthAggregator agg;

        HealthSnapshot s{};
        s.service_id = make_id(1);
        s.service_state = ServiceState::ONLINE;
        s.freshness = FreshnessState::FRESH;

        agg.observe_snapshot(s);

        auto health = agg.current_health();
        CHECK(health.level == SystemHealthLevel::HEALTHY);
        CHECK(health.healthy_services == 1);
    }

    // Test 9 — HealthAggregator degraded
    {
        HealthAggregator agg;

        HealthSnapshot s1{};
        s1.service_id = make_id(1);
        s1.service_state = ServiceState::ONLINE;
        agg.observe_snapshot(s1);

        HealthSnapshot s2{};
        s2.service_id = make_id(2);
        s2.service_state = ServiceState::DEGRADED;
        agg.observe_snapshot(s2);

        auto health = agg.current_health();
        CHECK(health.level == SystemHealthLevel::DEGRADED);
    }

    // Test 10 — HealthAggregator with failure
    {
        HealthAggregator agg;

        HealthSnapshot s{};
        s.service_id = make_id(1);
        s.service_state = ServiceState::ONLINE;
        agg.observe_snapshot(s);

        FailurePattern p{};
        p.pattern_id = make_id(10);
        p.failure_type = FailureType::DATA_FAILURE;
        agg.observe_failure(p);

        auto aggregate = agg.aggregate();
        CHECK(aggregate.services.size() == 1);
        CHECK(aggregate.recent_failures.size() == 1);
    }

    // Test 11 — ObservationOrchestrator end-to-end
    {
        ObservationOrchestrator orch;

        // 1. Record a prediction
        PredictionRecord pr{};
        pr.record_id = make_id(1);
        CHECK(orch.record_prediction(pr));
        CHECK(orch.prediction_count() == 1);

        // 2. Record an outcome
        OutcomeRecord orec{};
        orec.record_id = make_id(2);
        CHECK(orch.record_outcome(orec));
        CHECK(orch.outcome_count() == 1);

        // 3. Observe a failure
        FailurePattern fp{};
        fp.pattern_id = make_id(3);
        fp.failure_type = FailureType::SIGNAL_FAILURE;
        CHECK(orch.observe_failure(fp));
        CHECK(orch.failure_pattern_count() == 1);

        // 4. Observe health
        HealthSnapshot hs{};
        hs.service_id = make_id(4);
        hs.service_state = ServiceState::ONLINE;
        hs.freshness = FreshnessState::FRESH;
        orch.observe_health(hs);

        auto health = orch.current_health();
        CHECK(health.level == SystemHealthLevel::HEALTHY);

        auto agg = orch.health_aggregate();
        CHECK(agg.services.size() == 1);
        CHECK(agg.recent_failures.size() == 1);
    }

    // Test 12 — ObservationOrchestrator duplicate rejection
    {
        ObservationOrchestrator orch;

        PredictionRecord pr{};
        pr.record_id = make_id(1);

        CHECK(orch.record_prediction(pr));
        CHECK(!orch.record_prediction(pr));
        CHECK(orch.prediction_count() == 1);
    }

    // Test 13 — ObservationOrchestrator clear
    {
        ObservationOrchestrator orch;

        PredictionRecord pr{};
        pr.record_id = make_id(1);
        orch.record_prediction(pr);

        OutcomeRecord orec{};
        orec.record_id = make_id(2);
        orch.record_outcome(orec);

        CHECK(orch.prediction_count() == 1);
        CHECK(orch.outcome_count() == 1);

        orch.clear();

        CHECK(orch.prediction_count() == 0);
        CHECK(orch.outcome_count() == 0);
        CHECK(orch.failure_pattern_count() == 0);
    }

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    }

    std::cerr << g_failures << " TEST(S) FAILED\n";
    return 1;
}
