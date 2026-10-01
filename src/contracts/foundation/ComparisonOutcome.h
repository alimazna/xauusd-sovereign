#pragma once

#include <cstdint>

namespace xauusd::sovereign {

enum class ComparisonOutcome : std::uint8_t {
    BETTER,
    EQUAL,
    WORSE,
    INCONCLUSIVE,
    INCOMPARABLE
};

} // namespace xauusd::sovereign
