#include "ExperimentLedger.h"
#include "ExperimentStatus.h"
#include "FingerprintGenerator.h"
#include "HypothesisStore.h"
#include "HypothesisClass.h"
#include "HypothesisStatus.h"
#include "ResearchOrchestrator.h"
#include "ResearchPlanner.h"
#include "Sandbox.h"
#include "SandboxStatus.h"

#include <array>
#include <cstdint>
#include <string>
#include <vector>

using namespace xauusd::sovereign;

namespace {

int failures = 0;

#define CHECK(condition) \
    do { \
        if (!(condition)) { \
            ++failures; \
        } \
    } while (false)

static EntityId make_id(std::uint8_t seed) {
    std::array<std::uint8_t, 16> bytes{};
    bytes[0] = seed;
    return EntityId{bytes};
}

static Hypothesis make_hypothesis(std::uint8_t seed) {
    return Hypothesis{
        make_id(seed),
        KnowledgeId{make_id(static_cast<std::uint8_t>(seed + 1U))},
        EntityId{},
        HypothesisClass::RULE_HYPOTHESIS,
        HypothesisStatus::DRAFTED,
        "claim",
        "scope",
        "effect",
        "success",
        "failure",
        "no-effect",
        0.5,
        Timestamp{100},
        Version{1}};
}

static Experiment make_experiment(std::uint8_t seed, const ExperimentFingerprint& fp) {
    return Experiment{
        make_id(seed),
        make_id(static_cast<std::uint8_t>(seed + 1U)),
        Version{1},
        "dataset",
        "features",
        "evaluator",
        "environment",
        fp,
        ExperimentStatus::PLANNED,
        0,
        10,
        Timestamp{100},
        Timestamp{0}};
}

static ResearchBudget make_budget() {
    return ResearchBudget{10, 100, 20, 30, 40};
}

} // namespace

int main() {
    // 1. HypothesisStore: add + contains + size
    {
        HypothesisStore store;
        const Hypothesis h = make_hypothesis(1);
        CHECK(store.add(h));
        CHECK(store.contains(h.hypothesis_id));
        CHECK(store.size() == 1U);
    }

    // 2. HypothesisStore: duplicate rejection
    {
        HypothesisStore store;
        const Hypothesis h = make_hypothesis(2);
        CHECK(store.add(h));
        CHECK(!store.add(h));
        CHECK(store.size() == 1U);
    }

    // 3. HypothesisStore: get + update_status
    {
        HypothesisStore store;
        const Hypothesis h = make_hypothesis(3);
        Hypothesis out;
        CHECK(store.add(h));
        CHECK(store.get(h.hypothesis_id, out));
        CHECK(out.status == HypothesisStatus::DRAFTED);
        CHECK(store.update_status(h.hypothesis_id, HypothesisStatus::SUBMITTED));
        CHECK(store.get(h.hypothesis_id, out));
        CHECK(out.status == HypothesisStatus::SUBMITTED);
    }

    // 4. HypothesisStore: all + clear
    {
        HypothesisStore store;
        CHECK(store.add(make_hypothesis(4)));
        CHECK(store.add(make_hypothesis(5)));
        CHECK(store.all().size() == 2U);
        store.clear();
        CHECK(store.size() == 0U);
    }

    // 5. Hypothesis enum sanity (HypothesisClass, HypothesisStatus)
    {
        CHECK(static_cast<std::uint8_t>(HypothesisClass::FEATURE_HYPOTHESIS) == 0U);
        CHECK(static_cast<std::uint8_t>(HypothesisClass::UNKNOWN_HYPOTHESIS) == 11U);
        CHECK(static_cast<std::uint8_t>(HypothesisStatus::DRAFTED) == 0U);
        CHECK(static_cast<std::uint8_t>(HypothesisStatus::WITHDRAWN) == 6U);
    }

    // 6. ExperimentLedger: add + contains
    {
        ExperimentLedger ledger;
        Experiment e = make_experiment(10, ExperimentFingerprint{{1U, 2U}, "fp-1"});
        CHECK(ledger.add(e));
        CHECK(ledger.contains(e.experiment_id));
    }

    // 7. ExperimentLedger: fingerprint duplicate detection
    {
        ExperimentLedger ledger;
        const ExperimentFingerprint fp{{7U, 8U, 9U}, "fp"};
        Experiment e1 = make_experiment(11, fp);
        Experiment e2 = make_experiment(12, fp);
        CHECK(ledger.add(e1));
        CHECK(ledger.has_fingerprint(fp));
        CHECK(!ledger.add(e2));
    }

    // 8. ExperimentLedger: record_result + results_for
    {
        ExperimentLedger ledger;
        const ExperimentFingerprint fp{{1U, 3U}, "fp"};
        const Experiment e = make_experiment(13, fp);
        const ExperimentResult result{
            make_id(20),
            e.experiment_id,
            ExperimentStatus::COMPLETED,
            "done",
            0.8,
            0.2,
            Timestamp{200}};
        CHECK(ledger.add(e));
        CHECK(ledger.record_result(result));
        CHECK(ledger.results_for(e.experiment_id).size() == 1U);
        CHECK(!ledger.record_result(ExperimentResult{
            make_id(21), make_id(99), ExperimentStatus::FAILED, "bad", 0.0, 0.0, Timestamp{0}}));
    }

    // 9. ExperimentLedger: get + size
    {
        ExperimentLedger ledger;
        const Experiment e = make_experiment(14, ExperimentFingerprint{{4U}, "fp"});
        Experiment out;
        CHECK(ledger.add(e));
        CHECK(ledger.get(e.experiment_id, out));
        CHECK(out.experiment_id == e.experiment_id);
        CHECK(ledger.size() == 1U);
    }

    // 10. FingerprintGenerator: deterministic hash
    {
        const auto a = FingerprintGenerator::hash_components("deterministic");
        const auto b = FingerprintGenerator::hash_components("deterministic");
        CHECK(a == b);
        CHECK(a.size() == 16U);
        CHECK(a[0] == a[8]);
        CHECK(a[1] == a[9]);
        CHECK(a[2] == a[10]);
        CHECK(a[3] == a[11]);
        CHECK(a[4] == a[12]);
        CHECK(a[5] == a[13]);
        CHECK(a[6] == a[14]);
        CHECK(a[7] == a[15]);
    }

    // 11. FingerprintGenerator: different inputs → different fingerprints
    {
        const auto a = FingerprintGenerator::hash_components("a");
        const auto b = FingerprintGenerator::hash_components("b");
        CHECK(a != b);
    }

    // 12. ResearchPlanner: start_campaign + active_campaign_count
    {
        ResearchPlanner planner;
        const ResearchCampaign c{
            make_id(30), make_id(31), "goal", make_budget(), Timestamp{1}, Timestamp{2}, true};
        CHECK(planner.start_campaign(c));
        CHECK(planner.active_campaign_count() == 1U);
    }

    // 13. ResearchPlanner: stop_campaign
    {
        ResearchPlanner planner;
        const EntityId campaign_id = make_id(32);
        CHECK(planner.start_campaign(
            ResearchCampaign{campaign_id, make_id(33), "goal", make_budget(), Timestamp{1}, Timestamp{2}, true}));
        CHECK(planner.stop_campaign(campaign_id));
        CHECK(planner.active_campaign_count() == 0U);
        CHECK(!planner.stop_campaign(campaign_id));
    }

    // 14. ResearchPlanner: rank_priorities descending
    {
        ResearchPlanner planner;
        std::vector<ResearchPriority> candidates{
            ResearchPriority{make_id(40), 1.0, "a", 1, 0.1, Timestamp{1}},
            ResearchPriority{make_id(41), 3.0, "b", 2, 0.2, Timestamp{2}},
            ResearchPriority{make_id(42), 2.0, "c", 3, 0.3, Timestamp{3}},
            ResearchPriority{make_id(43), 3.0, "d", 4, 0.4, Timestamp{4}}};
        const auto ranked = planner.rank_priorities(candidates);
        CHECK(ranked.size() == 4U);
        CHECK(ranked[0].score == 3.0);
        CHECK(ranked[1].score == 3.0);
        CHECK(ranked[2].score == 2.0);
        CHECK(ranked[3].score == 1.0);
        CHECK(ranked[0].entity_id == candidates[1].entity_id);
        CHECK(ranked[1].entity_id == candidates[3].entity_id);
    }

    // 15. Sandbox: prepare + run lifecycle
    {
        Sandbox sandbox;
        const EntityId experiment_id = make_id(50);
        CHECK(sandbox.prepare(experiment_id, SandboxConfig{100, 1024, false, false, "x"}));
        SandboxRun run;
        CHECK(sandbox.get_run(experiment_id, run));
        CHECK(run.status == SandboxStatus::PREPARING);
        CHECK(sandbox.run(run.run_id));
        CHECK(sandbox.get_run(run.run_id, run));
        CHECK(run.status == SandboxStatus::COMPLETED);
    }

    // 16. Sandbox: abort transition
    {
        Sandbox sandbox;
        const EntityId experiment_id = make_id(51);
        CHECK(sandbox.prepare(experiment_id, SandboxConfig{0, 0, false, false, "abort"}));
        CHECK(sandbox.abort(experiment_id));
        SandboxRun run;
        CHECK(sandbox.get_run(experiment_id, run));
        CHECK(run.status == SandboxStatus::ABORTED);
    }

    // 17. Sandbox: run_count
    {
        Sandbox sandbox;
        CHECK(sandbox.prepare(make_id(52), SandboxConfig{}));
        CHECK(sandbox.prepare(make_id(53), SandboxConfig{}));
        CHECK(sandbox.run_count() == 2U);
    }

    // 18. ResearchOrchestrator: end-to-end flow
    {
        ResearchOrchestrator orchestrator;
        const Hypothesis h = make_hypothesis(60);
        CHECK(orchestrator.add_hypothesis(h));
        CHECK(orchestrator.update_hypothesis_status(h.hypothesis_id, HypothesisStatus::SUBMITTED));
        const Experiment bare = make_experiment(61, ExperimentFingerprint{});
        Experiment e = bare;
        e.fingerprint = orchestrator.generate_fingerprint(e);
        CHECK(orchestrator.add_experiment(e));
        CHECK(orchestrator.record_result(ExperimentResult{
            make_id(62), e.experiment_id, ExperimentStatus::COMPLETED, "ok", 1.0, 0.5, Timestamp{4}}));
        CHECK(orchestrator.start_campaign(
            ResearchCampaign{make_id(63), h.hypothesis_id, "goal", make_budget(), Timestamp{5}, Timestamp{6}, true}));
        CHECK(orchestrator.active_campaign_count() == 1U);
        CHECK(orchestrator.prepare_sandbox(e.experiment_id, SandboxConfig{}));
        CHECK(orchestrator.sandbox_run_count() == 1U);
    }

    // 19. ResearchOrchestrator: clear
    {
        ResearchOrchestrator orchestrator;
        const Hypothesis h = make_hypothesis(64);
        CHECK(orchestrator.add_hypothesis(h));
        CHECK(orchestrator.start_campaign(
            ResearchCampaign{make_id(65), h.hypothesis_id, "goal", make_budget(), Timestamp{1}, Timestamp{2}, true}));
        CHECK(orchestrator.prepare_sandbox(make_id(66), SandboxConfig{}));
        orchestrator.clear();
        CHECK(orchestrator.hypothesis_count() == 0U);
        CHECK(orchestrator.experiment_count() == 0U);
        CHECK(orchestrator.active_campaign_count() == 0U);
        CHECK(orchestrator.sandbox_run_count() == 0U);
    }

    // 20. ResearchOrchestrator: fingerprint propagation
    {
        ResearchOrchestrator orchestrator;
        Experiment e = make_experiment(67, ExperimentFingerprint{});
        const ExperimentFingerprint fp = orchestrator.generate_fingerprint(e);
        e.fingerprint = fp;
        CHECK(orchestrator.add_experiment(e));
        CHECK(orchestrator.has_experiment_fingerprint(fp));
    }

    #undef CHECK
    return failures == 0 ? 0 : 1;
}
