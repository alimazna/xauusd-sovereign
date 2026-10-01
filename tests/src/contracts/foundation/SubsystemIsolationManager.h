#pragma once

#include "IDependencyGraph.h"
#include "CapabilityId.h"

#include <set>
#include <vector>

namespace xauusd::sovereign {

class SubsystemIsolationManager {
public:
    explicit SubsystemIsolationManager(const IDependencyGraph* graph) noexcept;

    std::vector<CapabilityId> isolate(CapabilityId failed_capability);

    bool is_isolated(CapabilityId capability_id) const;

private:
    const IDependencyGraph* graph_;
    std::set<CapabilityId> isolated_;
};

} // namespace xauusd::sovereign
