#pragma once
#include <cstdint>
namespace xauusd::sovereign {
enum class ResourceType : std::uint8_t {
    CPU,
    RAM,
    DISK,
    NETWORK,
    CONCURRENT_JOBS,
    COMPUTE_UNITS
};
}
