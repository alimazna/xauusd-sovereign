#pragma once

#include <cstdint>

namespace xauusd::sovereign {

enum class ConfigurationScope : std::uint8_t {
    RUNTIME,
    RESEARCH,
    GOVERNANCE,
    SANDBOX
};

} // namespace xauusd::sovereign