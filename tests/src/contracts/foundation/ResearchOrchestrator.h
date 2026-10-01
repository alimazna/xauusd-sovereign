#pragma once

#include "HypothesisStore.h"
#include "ExperimentLedger.h"
#include "FingerprintGenerator.h"
#include "ResearchPlanner.h"
#include "Sandbox.h"

#include <cstddef>

namespace xauusd::sovereign {

class ResearchOrchestrator {
public:
    ResearchOrchestrator() = default;

    // Hypotheses
    bool add_hypothesis(const Hypothesis& h);
    bool update_hypothesis_status(const EntityId& id, HypothesisStatus s);
    std::size_t hypothesis_count() const noexcept;

    // Experiments
    bool add_experiment(const Experiment& e);
    bool record_result(const ExperimentResult& r);
    bool has_experiment_fingerprint(const ExperimentFingerprint& fp) const;
    std::size_t experiment_count() const noexcept;

    // Campaigns
    bool start_campaign(const ResearchCampaign& c);
    bool stop_campaign(const EntityId& campaign_id);
    std::size_t active_campaign_count() const noexcept;

    // Sandbox
    bool prepare_sandbox(const EntityId& experiment_id,
                         const SandboxConfig& config);
    std::size_t sandbox_run_count() const noexcept;

    // Fingerprint
    ExperimentFingerprint generate_fingerprint(const Experiment& e) const;

    // Reset
    void clear();

private:
    HypothesisStore      hypotheses_;
    ExperimentLedger     ledger_;
    FingerprintGenerator fingerprints_;
    ResearchPlanner      planner_;
    Sandbox              sandbox_;
};

} // namespace xauusd::sovereign
