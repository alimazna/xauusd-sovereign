#pragma once

#include "MT5AdapterManager.h"
#include "MT5MessageParser.h"
#include "MT5MessageBuilder.h"
#include "MT5Server.h"
#include "../../contracts/foundation/DecisionFlowResult.h"
#include "../../contracts/foundation/RiskProposal.h"
#include "../../contracts/foundation/EntityId.h"
#include "../../contracts/foundation/RuntimeEngine.h"

#include <cstdint>
#include <mutex>
#include <string>

namespace xauusd::mt5 {

class MT5Integration {
public:
    MT5Integration(
        std::uint16_t port,
        sovereign::RuntimeEngine& runtime,
        sovereign::ShadowLedger& ledger,
        std::string auth_token = {});

    ~MT5Integration();

    MT5Integration(const MT5Integration&) = delete;
    MT5Integration& operator=(const MT5Integration&) = delete;

    bool start();
    void stop();
    bool is_running() const;
    const MT5AdapterManager& adapters() const noexcept;

    // Explicit live-command boundary used by the application/decision layer.
    // MT5 EAs remain in SHADOW mode by default until their MQL input enables live orders.
    bool submit_live_decision(const std::string& timeframe,
                              const sovereign::DecisionFlowResult& result,
                              const std::string& comment = {});
    bool submit_new_order(const std::string& timeframe,
                          const sovereign::EntityId& command_id,
                          const std::string& symbol,
                          const std::string& side,
                          double volume,
                          double price,
                          double stop_loss,
                          double take_profit,
                          const std::string& comment = {});
    bool submit_close_order(const std::string& timeframe,
                            const sovereign::EntityId& command_id,
                            std::uint64_t ticket);
    bool submit_modify_order(const std::string& timeframe,
                             const sovereign::EntityId& command_id,
                             std::uint64_t ticket,
                             double stop_loss,
                             double take_profit);
    bool submit_cancel_order(const std::string& timeframe,
                             const sovereign::EntityId& command_id,
                             std::uint64_t ticket);

private:
    void handle_message(MT5Server::ClientId client_id, std::uint16_t type, const std::string& json);
    void record_transport_event(const std::string& name,
                                const std::string& subject,
                                const std::string& details);

    MT5Server server_;
    MT5AdapterManager adapters_;
    sovereign::RuntimeEngine& runtime_;
    sovereign::ShadowLedger& ledger_;
    mutable std::mutex handler_mutex_;
    std::string auth_token_;
    std::string bound_symbol_;
    bool symbol_bound_{false};
};

} // namespace xauusd::mt5
