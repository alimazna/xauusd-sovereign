#pragma once
#include "EntityId.h"
#include "SignalDirection.h"
#include "StrategyFamily.h"
#include "Timeframe.h"
#include "Timestamp.h"
#include <string>
namespace xauusd::sovereign {struct Signal{EntityId signal_id{};EntityId decision_id{};Timeframe trigger_timeframe{};Timestamp triggered_at{};SignalDirection direction{SignalDirection::NONE};StrategyFamily strategy{StrategyFamily::NONE};double entry_reference{0.0};double invalidating_price{0.0};std::string rationale;};}
