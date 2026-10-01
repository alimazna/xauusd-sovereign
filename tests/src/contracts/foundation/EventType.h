#pragma once

#include <cstdint>

namespace xauusd::sovereign {

enum class EventType : std::uint16_t {
    TICK,
    BAR_CLOSED,
    BAR_UPDATED,
    SYMBOL_SPEC_CHANGED,
    CONNECTION_CHANGED,
    HEARTBEAT,
    NEWS_EVENT,
    MACRO_EVENT,
    ACCOUNT_CHANGED,
    ORDER_EVENT,
    DEAL_EVENT,
    POSITION_EVENT,
    SYSTEM_ERROR,
    DATA_QUALITY_EVENT,
    TIMER,
    RECOVERY_REQUEST
};

} // namespace xauusd::sovereign