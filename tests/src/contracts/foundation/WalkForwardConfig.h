#pragma once

#include <cstdint>

namespace xauusd::sovereign {

struct WalkForwardConfig {
    std::uint64_t train_window_us;
    std::uint64_t validation_window_us;
    std::uint64_t forward_window_us;
    std::uint64_t step_us;
    std::uint64_t max_windows;

    WalkForwardConfig() = default;

    WalkForwardConfig(std::uint64_t train_window_us,
                      std::uint64_t validation_window_us,
                      std::uint64_t forward_window_us,
                      std::uint64_t step_us,
                      std::uint64_t max_windows)
        : train_window_us(train_window_us),
          validation_window_us(validation_window_us),
          forward_window_us(forward_window_us),
          step_us(step_us),
          max_windows(max_windows) {}
};

} // namespace xauusd::sovereign
