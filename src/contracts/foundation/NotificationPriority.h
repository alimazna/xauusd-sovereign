#pragma once

#include <cstdint>

namespace xauusd::sovereign {

enum class NotificationPriority : std::uint8_t {
    LOW,
    NORMAL,
    HIGH,
    URGENT,
    CRITICAL
};

} // namespace xauusd::sovereign
