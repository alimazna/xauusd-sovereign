#pragma once

#include <cstdint>

namespace xauusd::sovereign {

enum class ErrorSeverity : std::uint8_t {
    INFO,
    WARNING,
    ERROR,
    CRITICAL,
    FATAL
};

} // namespace xauusd::sovereign