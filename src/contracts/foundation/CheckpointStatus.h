#pragma once
#include <cstdint>
namespace xauusd::sovereign {
enum class CheckpointStatus : std::uint8_t {
    CREATED,
    VALIDATED,
    RESTORED,
    INVALID,
    EXPIRED
};
}
