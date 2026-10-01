#pragma once
#include "EntityId.h"
#include "PositionState.h"
#include "SignalDirection.h"
#include "Timestamp.h"
#include <string>
namespace xauusd::sovereign {
struct Position{
 EntityId position_id{};EntityId signal_id{};EntityId proposal_id{};EntityId fill_id{};SignalDirection direction{SignalDirection::NONE};PositionState state{PositionState::REJECTED};
 Timestamp opened_at{};Timestamp closed_at{};double requested_price{0.0};double entry_price{0.0};double exit_price{0.0};double volume{0.0};double stop_loss{0.0};double take_profit{0.0};double slippage{0.0};double gross_pnl{0.0};double net_pnl{0.0};double commission{0.0};double swap{0.0};double tick_size{0.0};double tick_value{0.0};std::string close_reason{};bool is_open{false};
};
}
