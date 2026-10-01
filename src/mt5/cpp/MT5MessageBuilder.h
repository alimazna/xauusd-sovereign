#pragma once

#include "../../contracts/foundation/EntityId.h"

#include <cstdint>
#include <string>

namespace xauusd::mt5 {

class MT5MessageBuilder {
public:
    static std::string build_handshake_ack(const std::string& timeframe);

    static std::string build_new_order(
        const sovereign::EntityId& command_id,
        const std::string& timeframe,
        const std::string& symbol,
        const std::string& side,
        double volume,
        double price,
        double stop_loss,
        double take_profit,
        const std::string& comment);

    static std::string build_close_order(
        const sovereign::EntityId& command_id,
        const std::string& timeframe,
        std::uint64_t ticket);

    static std::string build_modify_order(
        const sovereign::EntityId& command_id,
        const std::string& timeframe,
        std::uint64_t ticket,
        double stop_loss,
        double take_profit);

    static std::string build_cancel_order(
        const sovereign::EntityId& command_id,
        const std::string& timeframe,
        std::uint64_t ticket);

    static std::string build_heartbeat_ack(const std::string& timeframe);
    static std::string build_shutdown();
};

} // namespace xauusd::mt5
