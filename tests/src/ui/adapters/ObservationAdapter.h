#pragma once

#include "PredictionLedger.h"
#include "OutcomeEngine.h"
#include "FailureDetector.h"

#include <vector>

namespace xauusd::ui {

struct ObservationSourceStatus {
    bool predictions_bound{false};
    bool outcomes_bound{false};
    bool failures_bound{false};
};

struct ObservationSnapshot {
    std::vector<xauusd::sovereign::PredictionRecord> predictions;
    std::vector<xauusd::sovereign::OutcomeRecord> outcomes;
    std::vector<xauusd::sovereign::FailurePattern> failure_patterns;
    xauusd::sovereign::FailureReport failure_report{};
};

class ObservationAdapter {
public:
    ObservationAdapter(
        const xauusd::sovereign::PredictionLedger* predictions,
        const xauusd::sovereign::OutcomeEngine* outcomes,
        const xauusd::sovereign::FailureDetector* failures) noexcept;

    bool has_sources() const noexcept;
    ObservationSourceStatus sources() const noexcept;
    ObservationSnapshot snapshot() const;

    void bind(
        const xauusd::sovereign::PredictionLedger* predictions,
        const xauusd::sovereign::OutcomeEngine* outcomes,
        const xauusd::sovereign::FailureDetector* failures) noexcept;

private:
    const xauusd::sovereign::PredictionLedger* predictions_{nullptr};
    const xauusd::sovereign::OutcomeEngine* outcomes_{nullptr};
    const xauusd::sovereign::FailureDetector* failures_{nullptr};
};

} // namespace xauusd::ui
