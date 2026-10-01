#include "ObservationAdapter.h"

namespace xauusd::ui {

ObservationAdapter::ObservationAdapter(
    const xauusd::sovereign::PredictionLedger* predictions,
    const xauusd::sovereign::OutcomeEngine* outcomes,
    const xauusd::sovereign::FailureDetector* failures) noexcept
    : predictions_(predictions), outcomes_(outcomes), failures_(failures) {}

bool ObservationAdapter::has_sources() const noexcept {
    return predictions_ != nullptr || outcomes_ != nullptr || failures_ != nullptr;
}

ObservationSourceStatus ObservationAdapter::sources() const noexcept {
    return ObservationSourceStatus{
        predictions_ != nullptr,
        outcomes_ != nullptr,
        failures_ != nullptr
    };
}

ObservationSnapshot ObservationAdapter::snapshot() const {
    ObservationSnapshot result;
    if (predictions_ != nullptr) {
        result.predictions = predictions_->all();
    }
    if (outcomes_ != nullptr) {
        result.outcomes = outcomes_->all();
    }
    if (failures_ != nullptr) {
        result.failure_patterns = failures_->all_patterns();
        result.failure_report = failures_->generate_report();
    }
    return result;
}

void ObservationAdapter::bind(
    const xauusd::sovereign::PredictionLedger* predictions,
    const xauusd::sovereign::OutcomeEngine* outcomes,
    const xauusd::sovereign::FailureDetector* failures) noexcept {
    predictions_ = predictions;
    outcomes_ = outcomes;
    failures_ = failures;
}

} // namespace xauusd::ui
