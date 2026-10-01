#pragma once

#include <cstdint>

namespace xauusd::sovereign {

enum class Timeframe : std::uint8_t {
    M1,
    M5,
    M15,
    M30,
    H1,
    H4,
    D1,
    W1,
    MN1
};

} // namespace xauusd::sovereign
