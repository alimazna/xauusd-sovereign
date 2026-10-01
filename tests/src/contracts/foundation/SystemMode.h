#pragma once

#include <cstdint>

namespace xauusd::sovereign {

enum class SystemMode : std::uint8_t {
    STARTING,
    RECOVERY,
    NORMAL,
    DEGRADED,
    SHADOW,
    PAUSED,
    MANUAL,
    EMERGENCY,
    HALTED
};

} // namespace xauusd::sovereign