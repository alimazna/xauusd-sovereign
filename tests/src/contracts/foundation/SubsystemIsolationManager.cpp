#include "SubsystemIsolationManager.h"

#include <algorithm>

namespace xauusd::sovereign {

SubsystemIsolationManager::SubsystemIsolationManager(const IDependencyGraph* graph) noexcept
    : graph_(graph) {}

std::vector<CapabilityId> SubsystemIsolationManager::isolate(
    CapabilityId failed_capability) {
    std::vector<CapabilityId> result;
    if (graph_ == nullptr) {
        return result;
    }

    std::set<CapabilityId> visited;
    std::vector<CapabilityId> stack{failed_capability};
    while (!stack.empty()) {
        const CapabilityId current = stack.back();
        stack.pop_back();
        for (const CapabilityId dependent : graph_->direct_dependents_of(current)) {
            if (dependent == failed_capability) {
                continue;
            }
            if (!visited.insert(dependent).second) {
                continue;
            }
            isolated_.insert(dependent);
            result.push_back(dependent);
            stack.push_back(dependent);
        }
    }

    std::sort(result.begin(), result.end());
    return result;
}

bool SubsystemIsolationManager::is_isolated(CapabilityId capability_id) const {
    if (graph_ == nullptr) {
        return false;
    }
    return isolated_.find(capability_id) != isolated_.end();
}

} // namespace xauusd::sovereign
