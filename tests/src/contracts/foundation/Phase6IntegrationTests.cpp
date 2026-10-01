#include "EvidenceFirewall.h"
#include "ValidationFirewall.h"
#include "ValidationRunner.h"
#include "EvaluatorFirewall.h"
#include "ContaminationTracker.h"
#include "ValidationOrchestrator.h"

#include <array>
#include <cstdint>
#include <vector>

namespace xauusd::sovereign::tests {

namespace {

#define CHECK(expr) do { \
    if (!(expr)) { \
        return false; \
    } \
} while (false)

static EntityId make_id(std::uint8_t seed) {
    std::array<std::uint8_t, 16> bytes{};
    bytes[0] = seed;
    return EntityId{bytes};
}

static EvidenceLayer make_layer(EvidenceZone zone, std::uint8_t dataset_seed, bool locked) {
    return EvidenceLayer(
        zone,
        "layer",
        std::vector<EntityId>{make_id(dataset_seed)},
        Timestamp(1),
        Timestamp(2),
        locked);
}

static ValidationProtocol make_protocol(const EntityId& id) {
    return ValidationProtocol(
        id,
        "phase6",
        std::vector<ValidationMethod>{ValidationMethod::TIME_AWARE,
                                     ValidationMethod::WALK_FORWARD,
                                     ValidationMethod::MONTE_CARLO_STRESS},
        10,
        0.5,
        Version(1));
}

static bool test_1_evidence_register_contains() {
    EvidenceFirewall firewall(1);
    CHECK(firewall.register_layer(make_layer(EvidenceZone::E1_DEVELOPMENT, 1, false)));
    CHECK(firewall.contains_layer(EvidenceZone::E1_DEVELOPMENT));
    CHECK(!firewall.contains_layer(EvidenceZone::E2_OUT_OF_SAMPLE));
    return true;
}

static bool test_2_evidence_get() {
    EvidenceFirewall firewall(1);
    auto layer = make_layer(EvidenceZone::E2_OUT_OF_SAMPLE, 2, true);
    CHECK(firewall.register_layer(layer));
    EvidenceLayer out;
    CHECK(firewall.get_layer(EvidenceZone::E2_OUT_OF_SAMPLE, out));
    CHECK(out.locked);
    CHECK(out.dataset_ids.size() == 1);
    CHECK(out.dataset_ids.front() == make_id(2));
    return true;
}

static bool test_3_holdout_within_budget() {
    EvidenceFirewall firewall(2);
    HoldoutQuery q(make_id(1), make_id(2), make_id(3), Version(1), Timestamp(10), "test", false, true);
    CHECK(firewall.request_holdout(q));
    CHECK(firewall.holdout_query_count() == 1);
    CHECK(firewall.holdout_budget_remaining() == 2);
    return true;
}

static bool test_4_holdout_budget_exhaustion() {
    EvidenceFirewall firewall(1);
    CHECK(firewall.request_holdout(HoldoutQuery(make_id(1), make_id(2), make_id(3), Version(1), Timestamp(10), "a", false, false)));
    CHECK(firewall.consume_holdout(make_id(1)));
    CHECK(firewall.holdout_budget_remaining() == 0);
    CHECK(!firewall.request_holdout(HoldoutQuery(make_id(2), make_id(2), make_id(3), Version(1), Timestamp(10), "b", false, false)));
    CHECK(firewall.holdout_query_count() == 1);
    return true;
}

static bool test_5_consume_holdout() {
    EvidenceFirewall firewall(2);
    CHECK(firewall.request_holdout(HoldoutQuery(make_id(1), make_id(2), make_id(3), Version(1), Timestamp(10), "a", false, false)));
    CHECK(firewall.consume_holdout(make_id(1)));
    CHECK(firewall.holdout_budget_remaining() == 1);
    CHECK(!firewall.consume_holdout(make_id(1)));
    return true;
}

static bool test_6_snapshot() {
    EvidenceFirewall firewall(1);
    EvidenceSnapshot snapshot(make_id(1), make_id(9), EvidenceZone::E0_EXPLORATION,
                              std::vector<EntityId>{make_id(2)}, Version(3), Version(4), Timestamp(99));
    CHECK(firewall.record_snapshot(snapshot));
    const auto snapshots = firewall.snapshots_for(make_id(9));
    CHECK(snapshots.size() == 1);
    CHECK(snapshots.front().metric_registry_version == Version(3));
    CHECK(firewall.snapshots_for(make_id(8)).empty());
    return true;
}

static bool test_7_validation_protocol() {
    ValidationFirewall firewall;
    const auto protocol = make_protocol(make_id(1));
    CHECK(firewall.register_protocol(protocol));
    ValidationProtocol out;
    CHECK(firewall.get_protocol(make_id(1), out));
    CHECK(out.max_trials == 10);
    CHECK(out.methods.size() == 3);
    CHECK(firewall.protocol_count() == 1);
    return true;
}

static bool test_8_validation_run_result() {
    ValidationFirewall firewall;
    const auto protocol = make_protocol(make_id(1));
    CHECK(firewall.register_protocol(protocol));
    ValidationRun run(make_id(2), make_id(3), protocol, Timestamp(1), Timestamp(2), true, "ok");
    CHECK(firewall.record_run(run));
    ValidationResult result(make_id(4), make_id(2), ValidationMethod::TIME_AWARE,
                            true, 1.0, 0.5, 5, "pass", Timestamp(2));
    CHECK(firewall.record_result(result));
    CHECK(firewall.run_count() == 1);
    CHECK(firewall.results_for(make_id(2)).size() == 1);
    return true;
}

static bool test_9_all_passed_true() {
    ValidationFirewall firewall;
    const auto protocol = make_protocol(make_id(1));
    CHECK(firewall.register_protocol(protocol));
    CHECK(firewall.record_run(ValidationRun(make_id(2), make_id(3), protocol, Timestamp(1), Timestamp(2), true, "ok")));
    CHECK(firewall.record_result(ValidationResult(make_id(4), make_id(2), ValidationMethod::TIME_AWARE,
                                                  true, 1.0, 0.5, 1, "pass", Timestamp(2))));
    CHECK(firewall.record_result(ValidationResult(make_id(5), make_id(2), ValidationMethod::WALK_FORWARD,
                                                  true, 1.0, 0.5, 1, "pass", Timestamp(2))));
    CHECK(firewall.all_passed(make_id(2)));
    return true;
}

static bool test_10_all_passed_false() {
    ValidationFirewall firewall;
    const auto protocol = make_protocol(make_id(1));
    CHECK(firewall.register_protocol(protocol));
    CHECK(firewall.record_run(ValidationRun(make_id(2), make_id(3), protocol, Timestamp(1), Timestamp(2), true, "ok")));
    CHECK(firewall.record_result(ValidationResult(make_id(4), make_id(2), ValidationMethod::TIME_AWARE,
                                                  true, 1.0, 0.5, 1, "pass", Timestamp(2))));
    CHECK(firewall.record_result(ValidationResult(make_id(5), make_id(2), ValidationMethod::WALK_FORWARD,
                                                  false, 0.0, 0.5, 1, "fail", Timestamp(2))));
    CHECK(!firewall.all_passed(make_id(2)));
    return true;
}

static bool test_11_results_for() {
    ValidationFirewall firewall;
    const auto protocol = make_protocol(make_id(1));
    CHECK(firewall.register_protocol(protocol));
    CHECK(firewall.record_run(ValidationRun(make_id(2), make_id(3), protocol, Timestamp(1), Timestamp(2), true, "ok")));
    CHECK(firewall.record_result(ValidationResult(make_id(4), make_id(2), ValidationMethod::TIME_AWARE,
                                                  true, 1.0, 0.5, 1, "pass", Timestamp(2))));
    const auto results = firewall.results_for(make_id(2));
    CHECK(results.size() == 1);
    CHECK(results.front().method == ValidationMethod::TIME_AWARE);
    CHECK(firewall.results_for(make_id(8)).empty());
    return true;
}

static bool test_12_runner_oos() {
    ValidationRunner runner;
    const auto result = runner.run_oos(make_id(1), OOSWindow(make_id(2), make_id(3), Timestamp(1), Timestamp(2), 500, true));
    CHECK(result.method == ValidationMethod::TIME_AWARE);
    CHECK(result.passed);
    CHECK(result.score > 0.0);
    CHECK(runner.total_runs() == 1);
    return true;
}

static bool test_13_runner_walk_forward() {
    ValidationRunner runner;
    const auto results = runner.run_walk_forward(make_id(1), WalkForwardConfig(100, 200, 300, 50, 3));
    CHECK(results.size() == 3);
    CHECK(results.front().method == ValidationMethod::WALK_FORWARD);
    CHECK(results.front().passed);
    CHECK(runner.total_runs() == results.size());
    return true;
}

static bool test_14_runner_stress() {
    ValidationRunner runner;
    const auto result = runner.run_stress(make_id(1), StressTestConfig(100, 1.0, 1.0, 1.0, 0.25));
    CHECK(result.method == ValidationMethod::MONTE_CARLO_STRESS);
    CHECK(result.passed);
    CHECK(result.trials == 100);
    CHECK(runner.total_runs() == 1);
    return true;
}

static bool test_15_evaluator_register_contains_get() {
    EvaluatorFirewall firewall;
    const auto evaluator = EvaluatorIdentity(make_id(1), EvaluatorVersion(Version(2)),
                                             MetricRegistryVersion(Version(3)), "test");
    CHECK(firewall.register_evaluator(evaluator));
    CHECK(firewall.contains_evaluator(make_id(1)));
    EvaluatorIdentity out;
    CHECK(firewall.get_evaluator(make_id(1), out));
    CHECK(out.description == "test");
    CHECK(firewall.evaluator_count() == 1);
    return true;
}

static bool test_16_evaluator_freeze() {
    EvaluatorFirewall firewall;
    CHECK(firewall.register_evaluator(EvaluatorIdentity(make_id(1), EvaluatorVersion(Version(1)),
                                                       MetricRegistryVersion(Version(1)), "test")));
    CHECK(firewall.freeze(make_id(1)));
    CHECK(firewall.is_frozen(make_id(1)));
    CHECK(firewall.freeze(make_id(1)));
    return true;
}

static bool test_17_freeze_unregistered() {
    EvaluatorFirewall firewall;
    CHECK(!firewall.freeze(make_id(99)));
    CHECK(!firewall.is_frozen(make_id(99)));
    return true;
}

static bool test_18_contamination() {
    ContaminationTracker tracker;
    CHECK(tracker.mark(make_id(1), ContaminationState::OOS_EXPOSED));
    ContaminationState out = ContaminationState::CLEAN;
    CHECK(tracker.get(make_id(1), out));
    CHECK(out == ContaminationState::OOS_EXPOSED);
    CHECK(tracker.contains(make_id(1)));
    CHECK(tracker.size() == 1);
    return true;
}

static bool test_19_orchestrator_end_to_end() {
    ValidationOrchestrator orchestrator;
    CHECK(orchestrator.register_evidence_layer(make_layer(EvidenceZone::E3_LOCKED_HOLDOUT, 4, true)));
    CHECK(orchestrator.holdout_budget_remaining() == 0);

    const auto protocol = make_protocol(make_id(5));
    CHECK(orchestrator.register_protocol(protocol));
    CHECK(orchestrator.record_validation_run(ValidationRun(make_id(6), make_id(7), protocol,
                                                           Timestamp(1), Timestamp(2), true, "ok")));
    CHECK(orchestrator.record_validation_result(ValidationResult(make_id(8), make_id(6),
                                                                 ValidationMethod::TIME_AWARE,
                                                                 true, 1.0, 0.5, 1, "pass", Timestamp(2))));
    CHECK(orchestrator.candidate_passed(make_id(6)));

    CHECK(orchestrator.register_evaluator(EvaluatorIdentity(make_id(9), EvaluatorVersion(Version(1)),
                                                            MetricRegistryVersion(Version(1)), "phase6")));
    CHECK(orchestrator.freeze_evaluator(make_id(9)));
    CHECK(orchestrator.mark_contaminated(make_id(7), ContaminationState::VALIDATED));
    CHECK(orchestrator.contamination_state(make_id(7)) == ContaminationState::VALIDATED);
    return true;
}

static bool test_20_orchestrator_clear() {
    ValidationOrchestrator orchestrator;
    CHECK(orchestrator.register_evidence_layer(make_layer(EvidenceZone::E1_DEVELOPMENT, 1, false)));
    CHECK(orchestrator.register_evaluator(EvaluatorIdentity(make_id(4), EvaluatorVersion(Version(1)),
                                                            MetricRegistryVersion(Version(1)), "phase6")));
    CHECK(orchestrator.mark_contaminated(make_id(5), ContaminationState::CONTAMINATED));
    orchestrator.clear();
    CHECK(orchestrator.holdout_budget_remaining() == 0);
    CHECK(orchestrator.contamination_state(make_id(5)) == ContaminationState::CLEAN);
    return true;
}

} // namespace

} // namespace xauusd::sovereign::tests

int main() {
    using namespace xauusd::sovereign::tests;
    const bool results[] = {
        test_1_evidence_register_contains(),
        test_2_evidence_get(),
        test_3_holdout_within_budget(),
        test_4_holdout_budget_exhaustion(),
        test_5_consume_holdout(),
        test_6_snapshot(),
        test_7_validation_protocol(),
        test_8_validation_run_result(),
        test_9_all_passed_true(),
        test_10_all_passed_false(),
        test_11_results_for(),
        test_12_runner_oos(),
        test_13_runner_walk_forward(),
        test_14_runner_stress(),
        test_15_evaluator_register_contains_get(),
        test_16_evaluator_freeze(),
        test_17_freeze_unregistered(),
        test_18_contamination(),
        test_19_orchestrator_end_to_end(),
        test_20_orchestrator_clear()
    };

    for (bool ok : results) {
        if (!ok) {
            return 1;
        }
    }
    return 0;
}
