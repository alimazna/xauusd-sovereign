#pragma once
#include "CapabilityId.h"
#include <cstdint>
#include <string>
#include <utility>
namespace xauusd::sovereign {
enum class CriticalityLevel : std::uint8_t { CRITICAL, IMPORTANT, OPTIONAL };
struct CapabilityDescriptor {
    CapabilityId capability_id{};
    std::string capability_name{};
    CriticalityLevel criticality{CriticalityLevel::OPTIONAL};
    bool can_run_in_degraded{false};
    bool can_run_in_shadow{false};
    CapabilityDescriptor() = default;
    CapabilityDescriptor(CapabilityId id, std::string name, CriticalityLevel level, bool degraded, bool shadow)
        : capability_id(id), capability_name(std::move(name)), criticality(level),
          can_run_in_degraded(degraded), can_run_in_shadow(shadow) {}
};
}
