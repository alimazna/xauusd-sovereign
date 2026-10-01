#pragma once

#include "ValidationOutcome.h"
#include "DataQualityState.h"
#include "Timestamp.h"

namespace xauusd::sovereign
{

struct DataValidationResult
{
    ValidationOutcome   outcome{ValidationOutcome::UNKNOWN_VALIDATION_VALUE};
    DataQualityState    quality{DataQualityState::UNKNOWN_DATA_QUALITY};
    const char*         reason{nullptr};
    Timestamp           observed_at{};

    constexpr DataValidationResult() = default;

    constexpr explicit DataValidationResult(
        ValidationOutcome value)
        : outcome(value)
    {
    }

    constexpr DataValidationResult(
        ValidationOutcome   outcome_value,
        DataQualityState    quality_value,
        const char*         reason_value,
        Timestamp           observed_at_value)
        : outcome(outcome_value),
          quality(quality_value),
          reason(reason_value),
          observed_at(observed_at_value)
    {
    }

    constexpr bool passed() const
    {
        return outcome == ValidationOutcome::ACCEPTED_VALIDATION;
    }
};

}
