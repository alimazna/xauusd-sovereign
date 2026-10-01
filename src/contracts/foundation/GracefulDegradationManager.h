#pragma once
#include "IDegradationManager.h"
#include "IDependencyGraph.h"
#include "ICapabilityRegistry.h"
#include <set>
namespace xauusd::sovereign {
class GracefulDegradationManager final : public IDegradationManager {
public:
    GracefulDegradationManager(const IDependencyGraph* graph, const ICapabilityRegistry* registry) noexcept;
    std::vector<DegradationImpact> evaluate_impact(CapabilityId failed_capability) const override;
    bool is_capability_available(CapabilityId capability_id) const override;
    void mark_available(CapabilityId capability_id);
    void mark_unavailable(CapabilityId capability_id);
private:
    const IDependencyGraph* graph_;
    const ICapabilityRegistry* registry_;
    std::set<CapabilityId> unavailable_;
    std::set<CapabilityId> available_;
};
}
