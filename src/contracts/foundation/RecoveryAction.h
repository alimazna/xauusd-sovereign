#pragma once

#include <cstdint>

namespace xauusd::sovereign {

enum class RecoveryAction : std::uint8_t {
    NONE,
    RETRY,
    RECONNECT,
    RESTART_COMPONENT,
    ENTER_DEGRADED,
    PAUSE,
    RESUME,
    ROLLBACK,
    HALT,
    MANUAL_INTERVENTION_REQUIRED
};

} // namespace xauusd::sovereign