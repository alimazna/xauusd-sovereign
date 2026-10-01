#pragma once
#include "EntityId.h"
#include "FillState.h"
#include "SignalDirection.h"
#include "Timestamp.h"
namespace xauusd::sovereign {
struct SimulatedFill{
 EntityId fill_id{}; EntityId proposal_id{}; EntityId signal_id{}; Timestamp fill_time{};
 FillState state{FillState::REJECTED}; SignalDirection direction{SignalDirection::NONE};
 double requested_price{0.0}; double fill_price{0.0}; double volume{0.0}; double slippage{0.0};
 double commission{0.0}; double swap{0.0}; double stop_loss{0.0}; double take_profit{0.0};
 double tick_size{0.0}; double tick_value{0.0};
};
}
