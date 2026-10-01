#pragma once
#include <cstdint>
namespace xauusd::sovereign {
enum class IncidentSeverity : std::uint8_t {
    INFO,
    LOW,
    MEDIUM,
    HIGH,
    CRITICAL
};
}
