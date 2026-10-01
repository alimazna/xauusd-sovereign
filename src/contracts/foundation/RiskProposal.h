#pragma once
#include "EntityId.h"
#include "Signal.h"
#include "Timestamp.h"
namespace xauusd::sovereign {
struct RiskProposal{
 EntityId proposal_id{}; EntityId signal_id{}; Timestamp proposed_at{};
 SignalDirection direction{SignalDirection::NONE};
 double entry_price{0.0}; double volume{0.0}; double stop_loss{0.0}; double take_profit{0.0};
 double risk_amount{0.0}; double risk_fraction{0.0};
 double tick_size{0.0}; double tick_value{0.0};
 bool approved{false};
};
}
