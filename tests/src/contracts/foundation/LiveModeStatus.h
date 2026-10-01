#pragma once

#include <cstdint>

namespace xauusd::sovereign {

enum class LiveModeStatus : std::uint8_t {
    DISABLED,
    MANUAL_ONLY,
    SHADOW,
    CANARY,
    RESTRICTED,
    FULL,
    HALTED
};

} // namespace xauusd::sovereign
