#pragma once
#include <cstdint>

namespace xauusd::sovereign {

enum class EvidenceZone : std::uint8_t {
    E0_EXPLORATION,
    E1_DEVELOPMENT,
    E2_OUT_OF_SAMPLE,
    E3_LOCKED_HOLDOUT
};

} // namespace xauusd::sovereign
