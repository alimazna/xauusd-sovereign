#pragma once

#include <cstdint>

namespace xauusd::sovereign {

enum class SystemHealthLevel : std::uint8_t {
    HEALTHY,
    DEGRADED,
    CRITICAL,
    FAILED,
    UNKNOWN
};

} // namespace xauusd::sovereign
