#pragma once

#include "PredictionLedger.h"
#include "OutcomeEngine.h"
#include "FailureDetector.h"
#include "HealthAggregator.h"
#include "PredictionRecord.h"
#include "OutcomeRecord.h"
#include "FailurePattern.h"
#include "HealthSnapshot.h"

#include <cstddef>

namespace xauusd::sovereign {

class ObservationOrchestrator {
public:
    ObservationOrchestrator() = default;

    // Prediction
    bool record_prediction(const PredictionRecord& record);
    std::size_t prediction_count() const noexcept;
    const PredictionLedger& predictions() const noexcept;

    // Outcome
    bool record_outcome(const OutcomeRecord& record);
    std::size_t outcome_count() const noexcept;
    const OutcomeEngine& outcomes() const noexcept;

    // Failure
    bool observe_failure(const FailurePattern& pattern);
    std::size_t failure_pattern_count() const noexcept;
    const FailureDetector& failures() const noexcept;

    // Health
    void observe_health(const HealthSnapshot& snapshot);
    SystemHealthSnapshot current_health() const;
    HealthAggregate health_aggregate() const;

    // Reset (test helper)
    void clear();

private:
    PredictionLedger  predictions_;
    OutcomeEngine     outcomes_;
    FailureDetector   failures_;
    HealthAggregator  health_;
};

} // namespace xauusd::sovereign
