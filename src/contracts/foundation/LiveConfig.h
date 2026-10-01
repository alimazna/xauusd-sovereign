#pragma once
#include <cstdint>
#include <string>
#include <utility>
namespace xauusd::sovereign {
struct LiveConfig {
    std::uint64_t max_risk_per_trade_bps{0}; std::uint64_t max_daily_loss_bps{0}; std::uint64_t max_total_exposure_bps{0}; std::uint64_t max_concurrent_positions{0};
    std::string broker_symbol{}; std::string account_currency{}; bool require_confirmation{true};
    LiveConfig() = default;
    LiveConfig(std::uint64_t max_risk_per_trade_bps,std::uint64_t max_daily_loss_bps,std::uint64_t max_total_exposure_bps,std::uint64_t max_concurrent_positions,std::string broker_symbol,std::string account_currency,bool require_confirmation)
        : max_risk_per_trade_bps(max_risk_per_trade_bps),max_daily_loss_bps(max_daily_loss_bps),max_total_exposure_bps(max_total_exposure_bps),max_concurrent_positions(max_concurrent_positions),broker_symbol(std::move(broker_symbol)),account_currency(std::move(account_currency)),require_confirmation(require_confirmation) {}
};
}
