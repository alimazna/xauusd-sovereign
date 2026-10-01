#pragma once
#include <cstdint>
namespace xauusd::sovereign {
enum class RecoveryPhase : std::uint8_t {
    NONE,
    DETECTED,
    LOADING_CHECKPOINT,
    VALIDATING,
    RESTORING,
    COMPLETED,
    FAILED,
    ABORTED
};
}
