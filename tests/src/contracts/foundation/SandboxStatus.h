#pragma once

#include <cstdint>

namespace xauusd::sovereign {

enum class SandboxStatus : std::uint8_t {
    IDLE,
    PREPARING,
    RUNNING,
    COMPLETED,
    FAILED,
    ABORTED
};

} // namespace xauusd::sovereign
