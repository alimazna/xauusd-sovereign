#pragma once

#include <cstdint>

namespace xauusd::sovereign {

enum class PredictionDirection : std::uint8_t {
    UP,
    DOWN,
    NEUTRAL,
    UNKNOWN
};

} // namespace xauusd::sovereign
