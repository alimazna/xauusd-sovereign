#pragma once
#include <cstdint>
namespace xauusd::sovereign {enum class PositionState:std::uint8_t{PENDING_OPEN,OPEN,BREAKEVEN,TRAILING,PARTIAL_CLOSED,CLOSED,REJECTED};}
