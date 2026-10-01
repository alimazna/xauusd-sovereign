#pragma once
#include <cstdint>
namespace xauusd::sovereign {
enum class OperatingMode : std::uint8_t {
    OFFLINE,
    SCHEDULED,
    STARTING,
    ACTIVE,
    PAUSED,
    DRAINING,
    SAFE_SHUTDOWN,
    EMERGENCY_STOP
};
}
