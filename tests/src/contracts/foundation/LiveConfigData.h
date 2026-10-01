#pragma once

#include <cstdint>
#include <string>

namespace xauusd::sovereign {
namespace live_config_data {

// =====================================================================
// USER-CONFIGURABLE LIVE TRADING SETTINGS
// =====================================================================
// Fill in the values below with your own MT5 / broker data.
// These are NOT to be committed to any public repository.
// =====================================================================

// MT5 broker connection
constexpr const char* kBrokerName = "";
constexpr const char* kServerName = "";
constexpr const char* kAccountNumber = "";
constexpr const char* kAccountCurrency = "USD";

// Symbol
constexpr const char* kSymbol = "XAUUSD";
constexpr int         kDigits = 2;
constexpr double      kPoint = 0.01;

// Risk limits (basis points)
constexpr std::uint64_t kMaxRiskPerTradeBps = 10;
constexpr std::uint64_t kMaxDailyLossBps = 100;
constexpr std::uint64_t kMaxTotalExposureBps = 500;
constexpr std::uint64_t kMaxConcurrentPositions = 3;

// Canary
constexpr std::uint64_t kMinTradesBeforePromotion = 100;
constexpr std::uint64_t kMaxDrawdownBpsBeforeRollback = 200;

// Safety
constexpr bool kRequireConfirmation = true;
constexpr bool kEnableAutoKillSwitch = true;
constexpr bool kEnableLiveTrading = false;

// Note: actual MT5 bridge implementation is deferred.
// This config only defines parameters for when it is implemented.

} // namespace live_config_data
} // namespace xauusd::sovereign
