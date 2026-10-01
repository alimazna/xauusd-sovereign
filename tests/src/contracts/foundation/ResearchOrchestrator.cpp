#include "ResearchOrchestrator.h"

namespace xauusd::sovereign {

bool ResearchOrchestrator::add_hypothesis(const Hypothesis& h) {
    return hypotheses_.add(h);
}

bool ResearchOrchestrator::update_hypothesis_status(const EntityId& id,
                                                    HypothesisStatus s) {
    return hypotheses_.update_status(id, s);
}

std::size_t ResearchOrchestrator::hypothesis_count() const noexcept {
    return hypotheses_.size();
}

bool ResearchOrchestrator::add_experiment(const Experiment& e) {
    return ledger_.add(e);
}

bool ResearchOrchestrator::record_result(const ExperimentResult& r) {
    return ledger_.record_result(r);
}

bool ResearchOrchestrator::has_experiment_fingerprint(
    const ExperimentFingerprint& fp) const {
    return ledger_.has_fingerprint(fp);
}

std::size_t ResearchOrchestrator::experiment_count() const noexcept {
    return ledger_.size();
}

bool ResearchOrchestrator::start_campaign(const ResearchCampaign& c) {
    return planner_.start_campaign(c);
}

bool ResearchOrchestrator::stop_campaign(const EntityId& campaign_id) {
    return planner_.stop_campaign(campaign_id);
}

std::size_t ResearchOrchestrator::active_campaign_count() const noexcept {
    return planner_.active_campaign_count();
}

bool ResearchOrchestrator::prepare_sandbox(const EntityId& experiment_id,
                                           const SandboxConfig& config) {
    return sandbox_.prepare(experiment_id, config);
}

std::size_t ResearchOrchestrator::sandbox_run_count() const noexcept {
    return sandbox_.run_count();
}

ExperimentFingerprint ResearchOrchestrator::generate_fingerprint(
    const Experiment& e) const {
    return fingerprints_.generate(e);
}

void ResearchOrchestrator::clear() {
    hypotheses_.clear();
    ledger_.clear();
    planner_.clear();
    sandbox_.clear();
}

} // namespace xauusd::sovereign
