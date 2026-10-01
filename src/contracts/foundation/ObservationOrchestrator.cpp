#include "ObservationOrchestrator.h"

namespace xauusd::sovereign {

bool ObservationOrchestrator::record_prediction(
    const PredictionRecord& record) {
    return predictions_.append(record);
}

std::size_t ObservationOrchestrator::prediction_count() const noexcept {
    return predictions_.size();
}

const PredictionLedger& ObservationOrchestrator::predictions() const noexcept {
    return predictions_;
}

bool ObservationOrchestrator::record_outcome(
    const OutcomeRecord& record) {
    return outcomes_.record(record);
}

std::size_t ObservationOrchestrator::outcome_count() const noexcept {
    return outcomes_.size();
}

const OutcomeEngine& ObservationOrchestrator::outcomes() const noexcept {
    return outcomes_;
}

bool ObservationOrchestrator::observe_failure(
    const FailurePattern& pattern) {
    const bool observed = failures_.observe(pattern);
    health_.observe_failure(pattern);
    return observed;
}

std::size_t ObservationOrchestrator::failure_pattern_count() const noexcept {
    return failures_.pattern_count();
}

const FailureDetector& ObservationOrchestrator::failures() const noexcept {
    return failures_;
}

void ObservationOrchestrator::observe_health(
    const HealthSnapshot& snapshot) {
    health_.observe_snapshot(snapshot);
}

SystemHealthSnapshot ObservationOrchestrator::current_health() const {
    return health_.current_health();
}

HealthAggregate ObservationOrchestrator::health_aggregate() const {
    return health_.aggregate();
}

void ObservationOrchestrator::clear() {
    predictions_.clear();
    outcomes_.clear();
    failures_.clear();
    health_.clear();
}

} // namespace xauusd::sovereign
