
#pragma once

#include <cstdint>

namespace xauusd::sovereign {

enum class ErrorCode : std::uint16_t {
    NONE,
    DATA_ERROR,
    SCHEMA_ERROR,
    CLOCK_ERROR,
    CONNECTION_ERROR,
    BROKER_ERROR,
    RISK_ERROR,
    EXECUTION_ERROR,
    RECONCILIATION_ERROR,
    PERSISTENCE_ERROR,
    CONFIG_ERROR,
    INTEGRITY_ERROR,
    GUARDIAN_ERROR,
    INTERNAL_ERROR
};

} // namespace xauusd::sovereign