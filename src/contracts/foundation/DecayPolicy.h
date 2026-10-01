#pragma once

#include <cstdint>

namespace xauusd::sovereign {

struct DecayPolicy {
    std::int64_t  max_age_microseconds;
    std::int64_t  half_life_microseconds;
    double        revalidation_threshold;
    double        minimum_confidence;

    DecayPolicy() = default;

    DecayPolicy(
        std::int64_t max_age_microseconds,
        std::int64_t half_life_microseconds,
        double revalidation_threshold,
        double minimum_confidence)
        : max_age_microseconds(max_age_microseconds),
          half_life_microseconds(half_life_microseconds),
          revalidation_threshold(revalidation_threshold),
          minimum_confidence(minimum_confidence) {}
};

} // namespace xauusd::sovereign
