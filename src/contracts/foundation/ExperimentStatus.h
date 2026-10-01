#pragma once

#include <cstdint>

namespace xauusd::sovereign {

enum class ExperimentStatus : std::uint8_t {
    PLANNED,
    RUNNING,
    COMPLETED,
    FAILED,
    CANCELLED,
    INCONCLUSIVE,
    CONTAMINATED
};

} // namespace xauusd::sovereign
