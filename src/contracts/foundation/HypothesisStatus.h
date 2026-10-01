#pragma once

#include <cstdint>

namespace xauusd::sovereign {

enum class HypothesisStatus : std::uint8_t {
    DRAFTED,
    SUBMITTED,
    UNDER_EXPERIMENT,
    SUPPORTED,
    REFUTED,
    INCONCLUSIVE,
    WITHDRAWN
};

} // namespace xauusd::sovereign
