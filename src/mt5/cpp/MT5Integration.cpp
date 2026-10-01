#include "MT5Integration.h"

#include "../../contracts/foundation/AuditAction.h"
#include "../../contracts/foundation/ShadowLedgerEntry.h"
#include "../../contracts/foundation/ShadowDecision.h"
#include "../../contracts/foundation/Version.h"

#include <array>
#include <cstdlib>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string_view>

namespace xauusd::mt5 {
namespace {

std::int64_t now_ms() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

sovereign::EntityId make_entity_id(std::string_view prefix, std::string_view value) {
    // Stable, dependency-free 128-bit FNV-style identity for transport audit entries.
    std::array<std::uint64_t, 2> hashes{
        1469598103934665603ull,
        1099511628211ull ^ 0x9E3779B97F4A7C15ull};
    for (const auto ch : prefix) {
        const auto byte = static_cast<std::uint8_t>(ch);
        hashes[0] ^= byte; hashes[0] *= 1099511628211ull;
        hashes[1] ^= static_cast<std::uint8_t>(byte + 17u); hashes[1] *= 1099511628211ull;
    }
    for (const auto ch : value) {
        const auto byte = static_cast<std::uint8_t>(ch);
        hashes[0] ^= byte; hashes[0] *= 1099511628211ull;
        hashes[1] ^= static_cast<std::uint8_t>(byte + 31u); hashes[1] *= 1099511628211ull;
    }
    std::array<std::uint8_t, 16> bytes{};
    for (int i = 0; i < 8; ++i) {
        bytes[i] = static_cast<std::uint8_t>((hashes[0] >> (i * 8)) & 0xFFu);
        bytes[i + 8] = static_cast<std::uint8_t>((hashes[1] >> (i * 8)) & 0xFFu);
    }
    return sovereign::EntityId{bytes};
}


} // namespace

MT5Integration::MT5Integration(
    std::uint16_t port,
    sovereign::RuntimeEngine& runtime,
    sovereign::ShadowLedger& ledger,
    std::string auth_token)
    : server_(port), runtime_(runtime), ledger_(ledger),
      auth_token_(std::move(auth_token)) {
    if (auth_token_.empty()) {
        if (const char* env = std::getenv("XAUS_MT5_AUTH_TOKEN")) {
            auth_token_ = env;
        }
    }
    server_.set_message_handler([this](MT5Server::ClientId client_id,
                                       std::uint16_t type, const std::string& json) {
        handle_message(client_id, type, json);
    });
}

MT5Integration::~MT5Integration() {
    stop();
}

bool MT5Integration::start() {
    if (auth_token_.empty()) {
        std::cerr << "[MT5] Refusing to start: XAUS_MT5_AUTH_TOKEN is required\n";
        return false;
    }
    return server_.start();
}

void MT5Integration::stop() {
    if (!server_.is_running()) {
        return;
    }
    server_.send(static_cast<std::uint16_t>(MessageType::SHUTDOWN),
                 MT5MessageBuilder::build_shutdown());
    server_.stop();
    record_transport_event("MT5_SERVER_STOPPED", "localhost", "MT5 bridge stopped");
}

bool MT5Integration::is_running() const {
    return server_.is_running();
}

const MT5AdapterManager& MT5Integration::adapters() const noexcept {
    return adapters_;
}

void MT5Integration::handle_message(MT5Server::ClientId client_id, std::uint16_t type, const std::string& json) {
    std::lock_guard lock(handler_mutex_);
    const auto message_type = static_cast<MessageType>(type);
    try {
        switch (message_type) {
            case MessageType::HANDSHAKE: {
                const std::string tf = MT5MessageParser::extract_string(json, "tf");
                const std::string token = MT5MessageParser::extract_string(json, "token");
                const auto version = MT5MessageParser::extract_int64(json, "version");
                if (!MT5MessageParser::parse_timeframe(tf) ||
                    version != static_cast<std::int64_t>(kProtocolVersion) ||
                    token.empty() || token != auth_token_) {
                    std::cerr << "[MT5] Invalid/unauthorized HANDSHAKE for client " << client_id << "\n";
                    server_.disconnect_client(client_id);
                    return;
                }
                if (!server_.authenticate_client(client_id, tf)) {
                    std::cerr << "[MT5] Rejecting duplicate/invalid timeframe route: " << tf << "\n";
                    server_.disconnect_client(client_id);
                    return;
                }
                adapters_.register_adapter(tf);
                const std::string ack = MT5MessageBuilder::build_handshake_ack(tf);
                if (server_.send_to_client(client_id, static_cast<std::uint16_t>(MessageType::HANDSHAKE_ACK), ack) == 0) {
                    server_.disconnect_client(client_id);
                    return;
                }
                record_transport_event("MT5_HANDSHAKE", tf, json);
                break;
            }
            case MessageType::TICK: {
                const std::string tf = MT5MessageParser::extract_string(json, "tf");
                const std::string symbol = MT5MessageParser::extract_string(json, "sym");
                const auto tick = MT5MessageParser::parse_tick(json);
                if (!symbol_bound_ || !tick || !server_.client_matches_timeframe(client_id, tf) ||
                    symbol != bound_symbol_) {
                    std::cerr << "[MT5] Rejected TICK client/timeframe/symbol mismatch\n";
                    return;
                }
                adapters_.record_tick(tf);
                runtime_.ingest_tick(*tick);
                break;
            }
            case MessageType::BAR_CLOSED: {
                const std::string tf = MT5MessageParser::extract_string(json, "tf");
                const std::string symbol = MT5MessageParser::extract_string(json, "sym");
                const auto timeframe = MT5MessageParser::parse_timeframe(tf);
                const auto bar = MT5MessageParser::parse_bar(json);
                if (!symbol_bound_ || !timeframe || !bar || !server_.client_matches_timeframe(client_id, tf) ||
                    symbol != bound_symbol_) {
                    std::cerr << "[MT5] Rejected BAR client/timeframe/symbol mismatch\n";
                    return;
                }
                adapters_.record_bar(tf);
                (void)runtime_.process_bar(*timeframe, *bar);
                break;
            }
            case MessageType::SYMBOL_SPEC: {
                if (!server_.is_client_authenticated(client_id)) return;
                const auto spec = MT5MessageParser::parse_symbol_spec(json);
                if (!spec) {
                    std::cerr << "[MT5] Invalid SYMBOL_SPEC payload\n";
                    return;
                }
                if (symbol_bound_ && bound_symbol_ != spec->symbol) {
                    std::cerr << "[MT5] Rejecting cross-symbol SYMBOL_SPEC: " << spec->symbol << " expected "
                              << bound_symbol_ << "\n";
                    return;
                }
                if (!symbol_bound_) {
                    bound_symbol_ = spec->symbol;
                    symbol_bound_ = true;
                }
                runtime_.set_symbol_spec(*spec);
                record_transport_event("MT5_SYMBOL_SPEC", spec->symbol, json);
                break;
            }
            case MessageType::HEARTBEAT: {
                const std::string tf = MT5MessageParser::extract_string(json, "tf");
                if (!MT5MessageParser::parse_timeframe(tf)) {
                    std::cerr << "[MT5] Invalid HEARTBEAT timeframe: " << tf << "\n";
                    return;
                }
                if (!server_.client_matches_timeframe(client_id, tf)) return;
                adapters_.record_heartbeat(tf);
                (void)server_.send_to_client(client_id,
                             static_cast<std::uint16_t>(MessageType::HEARTBEAT_ACK),
                             MT5MessageBuilder::build_heartbeat_ack(tf));
                break;
            }
            case MessageType::COMMAND_RESULT: {
                const std::string cmd = MT5MessageParser::extract_string(json, "cmd");
                const std::string tf = MT5MessageParser::extract_string(json, "tf");
                const std::string client_tf = server_.client_timeframe(client_id);
                const std::string result_tf = tf.empty() ? client_tf : tf;
                const bool has_command_id = !cmd.empty();
                const auto timeframe = MT5MessageParser::parse_timeframe(result_tf);
                if (client_tf.empty() || !timeframe || result_tf != client_tf) {
                    return;
                }
                const auto entry_id = make_entity_id("mt5-command-result",
                                                     std::to_string(client_id) + "|" + json);
                const auto timestamp = sovereign::Timestamp{now_ms()};
                sovereign::ShadowDecision decision{
                    entry_id,
                    timestamp,
                    *timeframe,
                    "MT5_COMMAND_RESULT",
                    sovereign::Version{1},
                    sovereign::Version{1},
                    json};
                const bool appended = ledger_.append(
                    sovereign::ShadowLedgerEntry{entry_id, timestamp, std::move(decision),
                                                 "MT5 command result"});
                std::cerr << "[MT5] COMMAND_RESULT cmd=" << (has_command_id ? cmd : "<missing>")
                          << " ledger=" << (appended ? "recorded" : "duplicate/rejected") << "\n";
                break;
            }
            case MessageType::DISCONNECT: {
                const std::string tf = MT5MessageParser::extract_string(json, "tf");
                if (!tf.empty()) {
                    adapters_.unregister_adapter(tf);
                }
                record_transport_event("MT5_DISCONNECT", tf, json);
                break;
            }
            default:
                std::cerr << "[MT5] Unsupported message type 0x"
                          << std::hex << type << std::dec << "\n";
                break;
        }
    } catch (const std::exception& ex) {
        std::cerr << "[MT5] Handler exception for " << message_type_name(message_type)
                  << ": " << ex.what() << "\n";
    }
}


bool MT5Integration::submit_new_order(const std::string& timeframe,
                                      const sovereign::EntityId& command_id,
                                      const std::string& symbol,
                                      const std::string& side,
                                      double volume,
                                      double price,
                                      double stop_loss,
                                      double take_profit,
                                      const std::string& comment) {
    if (!server_.is_running() || !MT5MessageParser::parse_timeframe(timeframe)) return false;
    if (side != "BUY" && side != "SELL") return false;
    const auto payload = MT5MessageBuilder::build_new_order(
        command_id, timeframe, symbol, side, volume, price, stop_loss, take_profit, comment);
    return server_.send_to_timeframe(timeframe,
        static_cast<std::uint16_t>(MessageType::NEW_ORDER), payload) != 0;
}

bool MT5Integration::submit_live_decision(const std::string& timeframe,
                                           const sovereign::DecisionFlowResult& result,
                                           const std::string& comment) {
    if (!result.data_valid || !result.risk.approved || !result.completed) return false;
    std::string side;
    if (result.signal.direction == sovereign::SignalDirection::LONG) side = "BUY";
    else if (result.signal.direction == sovereign::SignalDirection::SHORT) side = "SELL";
    else return false;
    return submit_new_order(timeframe, result.risk.proposal_id, runtime_.symbol_spec().symbol,
                            side, result.risk.volume, result.risk.entry_price,
                            result.risk.stop_loss, result.risk.take_profit, comment);
}

bool MT5Integration::submit_close_order(const std::string& timeframe,
                                         const sovereign::EntityId& command_id,
                                         std::uint64_t ticket) {
    if (!server_.is_running() || ticket == 0) return false;
    return server_.send_to_timeframe(timeframe, static_cast<std::uint16_t>(MessageType::CLOSE_ORDER),
                                     MT5MessageBuilder::build_close_order(command_id, timeframe, ticket)) != 0;
}

bool MT5Integration::submit_modify_order(const std::string& timeframe,
                                          const sovereign::EntityId& command_id,
                                          std::uint64_t ticket, double stop_loss, double take_profit) {
    if (!server_.is_running() || ticket == 0) return false;
    return server_.send_to_timeframe(timeframe, static_cast<std::uint16_t>(MessageType::MODIFY_ORDER),
                                     MT5MessageBuilder::build_modify_order(command_id, timeframe, ticket, stop_loss, take_profit)) != 0;
}

bool MT5Integration::submit_cancel_order(const std::string& timeframe,
                                          const sovereign::EntityId& command_id,
                                          std::uint64_t ticket) {
    if (!server_.is_running() || ticket == 0) return false;
    return server_.send_to_timeframe(timeframe, static_cast<std::uint16_t>(MessageType::CANCEL_ORDER),
                                     MT5MessageBuilder::build_cancel_order(command_id, timeframe, ticket)) != 0;
}

void MT5Integration::record_transport_event(
    const std::string& name,
    const std::string& subject,
    const std::string& details) {
    const auto id = make_entity_id(name, subject + details);
    const auto at = sovereign::Timestamp{now_ms()};
    sovereign::ShadowDecision decision{
        id, at, sovereign::Timeframe::M1, name,
        sovereign::Version{1}, sovereign::Version{1}, details};
    (void)ledger_.append(sovereign::ShadowLedgerEntry{
        id, at, std::move(decision), "MT5 transport event: " + subject});
}

} // namespace xauusd::mt5
