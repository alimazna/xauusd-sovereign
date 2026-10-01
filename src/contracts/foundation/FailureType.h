#pragma once

#include <cstdint>

namespace xauusd::sovereign {

enum class FailureType : std::uint16_t {
    DATA_FAILURE,
    SCHEMA_FAILURE,
    CLOCK_FAILURE,
    CONNECTION_FAILURE,
    FEATURE_FAILURE,
    REGIME_FAILURE,
    ELIGIBILITY_FAILURE,
    SIGNAL_FAILURE,
    RISK_FAILURE,
    EXECUTION_FAILURE,
    RECONCILIATION_FAILURE,
    PERSISTENCE_FAILURE,
    UNKNOWN_FAILURE
};

} // namespace xauusd::sovereign
