#include "StaleCandleDetector.h"

namespace xauusd::sovereign {

StaleCandleDetector::StaleCandleDetector(
    std::int64_t max_age_microseconds) noexcept
    : max_age_microseconds_(max_age_microseconds) {}

bool StaleCandleDetector::is_stale(
    Timestamp candle_time,
    Timestamp now) const noexcept {
    if (now.value() <= candle_time.value()) {
        return false;
    }

    return (now.value() - candle_time.value()) > max_age_microseconds_;
}

std::int64_t StaleCandleDetector::max_age_microseconds() const noexcept {
    return max_age_microseconds_;
}

void StaleCandleDetector::set_max_age(
    std::int64_t max_age_microseconds) noexcept {
    max_age_microseconds_ = max_age_microseconds;
}

} // namespace xauusd::sovereign
