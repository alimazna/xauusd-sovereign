#pragma once

#include <cstdint>

namespace xauusd::sovereign {

enum class GuardianStatus : std::uint8_t {
    NORMAL,
    OBSERVE,
    WARN,
    DEGRADE,
    SAFE_MODE,
    HALT
};

} // namespace xauusd::sovereign
