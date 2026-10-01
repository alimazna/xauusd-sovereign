#pragma once
#include "CapabilityId.h"
#include "DegradationImpact.h"
#include <vector>
namespace xauusd::sovereign {
class IDegradationManager {
public:
    virtual ~IDegradationManager() = default;
    virtual std::vector<DegradationImpact> evaluate_impact(CapabilityId) const = 0;
    virtual bool is_capability_available(CapabilityId) const = 0;
};
}
