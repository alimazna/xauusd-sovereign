#include "MT5MessageBuilder.h"

#include <array>
#include <iomanip>
#include <cmath>
#include <sstream>

namespace xauusd::mt5 {
namespace {

std::string escape_json(const std::string& text) {
    std::string out;
    out.reserve(text.size() + 8);
    for (const char ch : text) {
        switch (ch) {
            case '\\': out += "\\\\"; break;
            case '"': out += "\\\""; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default: out += ch; break;
        }
    }
    return out;
}

std::string entity_id_hex(const sovereign::EntityId& id) {
    static constexpr std::array<char, 16> hex{
        '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    std::string out;
    out.reserve(32);
    for (const auto byte : id.bytes()) {
        out.push_back(hex[(byte >> 4u) & 0x0Fu]);
        out.push_back(hex[byte & 0x0Fu]);
    }
    return out;
}

std::string number(double value) {
    if (!std::isfinite(value)) return {};
    std::ostringstream out;
    out << std::setprecision(15) << value;
    return out.str();
}

} // namespace

std::string MT5MessageBuilder::build_handshake_ack(const std::string& timeframe) {
    return "{\"tf\":\"" + escape_json(timeframe) + "\",\"authenticated\":true}";
}

std::string MT5MessageBuilder::build_new_order(
    const sovereign::EntityId& command_id,
    const std::string& timeframe,
    const std::string& symbol,
    const std::string& side,
    double volume,
    double price,
    double stop_loss,
    double take_profit,
    const std::string& comment) {
    const auto volume_text = number(volume);
    const auto price_text = number(price);
    const auto sl_text = number(stop_loss);
    const auto tp_text = number(take_profit);
    if (volume_text.empty() || price_text.empty() || sl_text.empty() || tp_text.empty()) return {};
    std::ostringstream out;
    out << "{\"cmd\":\"" << entity_id_hex(command_id)
        << "\",\"tf\":\"" << escape_json(timeframe)
        << "\",\"sym\":\"" << escape_json(symbol)
        << "\",\"side\":\"" << escape_json(side)
        << "\",\"vol\":" << volume_text
        << ",\"price\":" << price_text
        << ",\"sl\":" << sl_text
        << ",\"tp\":" << tp_text
        << ",\"cmt\":\"" << escape_json(comment) << "\"}";
    return out.str();
}

std::string MT5MessageBuilder::build_close_order(
    const sovereign::EntityId& command_id,
    const std::string& timeframe,
    std::uint64_t ticket) {
    return "{\"cmd\":\"" + entity_id_hex(command_id) +
           "\",\"tf\":\"" + escape_json(timeframe) +
           "\",\"ticket\":" + std::to_string(ticket) + "}";
}

std::string MT5MessageBuilder::build_modify_order(
    const sovereign::EntityId& command_id,
    const std::string& timeframe,
    std::uint64_t ticket,
    double stop_loss,
    double take_profit) {
    const auto sl_text = number(stop_loss);
    const auto tp_text = number(take_profit);
    if (sl_text.empty() || tp_text.empty()) return {};
    return "{\"cmd\":\"" + entity_id_hex(command_id) +
           "\",\"tf\":\"" + escape_json(timeframe) +
           "\",\"ticket\":" + std::to_string(ticket) +
           ",\"sl\":" + sl_text +
           ",\"tp\":" + tp_text + "}";
}

std::string MT5MessageBuilder::build_cancel_order(
    const sovereign::EntityId& command_id,
    const std::string& timeframe,
    std::uint64_t ticket) {
    return "{\"cmd\":\"" + entity_id_hex(command_id) +
           "\",\"tf\":\"" + escape_json(timeframe) +
           "\",\"ticket\":" + std::to_string(ticket) + "}";
}

std::string MT5MessageBuilder::build_heartbeat_ack(const std::string& timeframe) {
    return "{\"tf\":\"" + escape_json(timeframe) + "\"}";
}
std::string MT5MessageBuilder::build_shutdown() { return "{}"; }

} // namespace xauusd::mt5
