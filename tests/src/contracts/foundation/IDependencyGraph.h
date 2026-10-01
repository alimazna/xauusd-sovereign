#pragma once
#include "CapabilityId.h"
#include <vector>
namespace xauusd::sovereign {
class IDependencyGraph {
public:
    virtual ~IDependencyGraph() = default;
    virtual std::vector<CapabilityId> direct_dependencies_of(CapabilityId) const = 0;
    virtual std::vector<CapabilityId> direct_dependents_of(CapabilityId) const = 0;
    virtual bool has_path(CapabilityId, CapabilityId) const = 0;
};
}
