#pragma once

#include <cstdint>

namespace xauusd::sovereign {

enum class CanaryStage : std::uint8_t {
    STAGE_0_OFF,
    STAGE_1_MINIMAL,
    STAGE_2_SMALL,
    STAGE_3_MEDIUM,
    STAGE_4_FULL
};

} // namespace xauusd::sovereign
