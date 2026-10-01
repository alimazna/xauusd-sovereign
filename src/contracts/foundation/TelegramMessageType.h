#pragma once

#include <cstdint>

namespace xauusd::sovereign {

enum class TelegramMessageType : std::uint8_t {
    STATUS_UPDATE,
    ALERT,
    REPORT,
    APPROVAL_REQUEST,
    INCIDENT,
    GOVERNANCE_NOTIFICATION,
    REPLY
};

} // namespace xauusd::sovereign
