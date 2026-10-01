#pragma once

#include "Timestamp.h"

#include <cstdint>

namespace xauusd::sovereign {

class StaleCandleDetector {
public:
    StaleCandleDetector() = default;

    explicit StaleCandleDetector(
        std::int64_t max_age_microseconds) noexcept;

    bool is_stale(
        Timestamp candle_time,
        Timestamp now) const noexcept;

    std::int64_t max_age_microseconds() const noexcept;

    void set_max_age(std::int64_t max_age_microseconds) noexcept;

private:
    std::int64_t max_age_microseconds_{0};
};

} // namespace xauusd::sovereign
