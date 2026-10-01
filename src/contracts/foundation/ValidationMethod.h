#pragma once
#include <cstdint>

namespace xauusd::sovereign {

enum class ValidationMethod : std::uint8_t {
    TIME_AWARE,
    PURGED_KFOLD,
    CPCV,
    WALK_FORWARD,
    MONTE_CARLO_STRESS,
    PBO_CSCV,
    DEFLATED_SHARPE,
    REALITY_CHECK
};

} // namespace xauusd::sovereign
