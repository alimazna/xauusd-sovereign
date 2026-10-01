#pragma once

#include <cstdint>

namespace xauusd::sovereign {

enum class PersistenceStatus : std::uint8_t {
    OK,
    NOT_FOUND,
    CONFLICT,
    UNAVAILABLE,
    TIMEOUT,
    CORRUPTED,
    REJECTED,
    UNKNOWN
};

} // namespace xauusd::sovereign
