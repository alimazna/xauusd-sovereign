#pragma once
#include "CapabilityId.h"
#include "Timestamp.h"
#include <cstdint>
#include <string>
#include <utility>
namespace xauusd::sovereign {
enum class ImpactLevel : std::uint8_t { NONE, PARTIAL, FULL };
struct DegradationImpact {
    CapabilityId capability_id{};
    ImpactLevel level{ImpactLevel::NONE};
    std::string reason{};
    Timestamp observed_at{};
    DegradationImpact() = default;
    DegradationImpact(CapabilityId id, ImpactLevel impact, std::string explanation, const Timestamp& observed)
        : capability_id(id), level(impact), reason(std::move(explanation)), observed_at(observed) {}
};
}
