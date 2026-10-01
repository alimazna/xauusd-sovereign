#pragma once

#include <cstdint>

namespace xauusd::sovereign {

struct StressTestConfig {
    std::uint64_t iterations;
    double        slippage_multiplier;
    double        spread_multiplier;
    double        commission_multiplier;
    double        max_drawdown_threshold;

    StressTestConfig() = default;

    StressTestConfig(std::uint64_t iterations, double slippage_multiplier,
                     double spread_multiplier, double commission_multiplier,
                     double max_drawdown_threshold)
        : iterations(iterations),
          slippage_multiplier(slippage_multiplier),
          spread_multiplier(spread_multiplier),
          commission_multiplier(commission_multiplier),
          max_drawdown_threshold(max_drawdown_threshold) {}
};

} // namespace xauusd::sovereign
