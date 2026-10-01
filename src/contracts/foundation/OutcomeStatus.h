#pragma once

#include <cstdint>

namespace xauusd::sovereign {

enum class OutcomeStatus : std::uint8_t {
    SUCCESS,
    FAILURE,
    NEUTRAL,
    PARTIAL,
    EXPIRED,
    UNKNOWN
};

} // namespace xauusd::sovereign
