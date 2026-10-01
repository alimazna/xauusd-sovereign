#pragma once

#include <cstdint>

namespace xauusd::sovereign {

enum class NotificationChannel : std::uint8_t {
    TELEGRAM,
    DESKTOP,
    AUDIT_LOG,
    NONE
};

} // namespace xauusd::sovereign
