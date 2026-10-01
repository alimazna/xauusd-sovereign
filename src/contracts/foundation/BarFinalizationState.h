#pragma once

#include <cstdint>

namespace xauusd::sovereign {

enum class BarFinalizationState : std::uint8_t {
    FORMING,
    CLOSED,
    FINALIZED,
    REJECTED,
    UNKNOWN
};

} // namespace xauusd::sovereign
