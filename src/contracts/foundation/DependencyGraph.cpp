#include "DependencyGraph.h"
namespace xauusd::sovereign {
bool DependencyGraph::add_dependency(const DependencyDescriptor& descriptor) {
    auto& dependencies = outgoing_[descriptor.dependent];
    const auto [it, inserted] = dependencies.insert(descriptor.dependency);
    static_cast<void>(it);
    if (inserted) incoming_[descriptor.dependency].insert(descriptor.dependent);
    return inserted;
}
bool DependencyGraph::remove_dependency(CapabilityId dependent, CapabilityId dependency) {
    const auto out_it = outgoing_.find(dependent);
    if (out_it == outgoing_.end() || out_it->second.erase(dependency) == 0) return false;
    const auto in_it = incoming_.find(dependency);
    if (in_it != incoming_.end()) {
        in_it->second.erase(dependent);
        if (in_it->second.empty()) incoming_.erase(in_it);
    }
    if (out_it->second.empty()) outgoing_.erase(out_it);
    return true;
}
std::vector<CapabilityId> DependencyGraph::direct_dependencies_of(CapabilityId capability_id) const {
    const auto it = outgoing_.find(capability_id);
    if (it == outgoing_.end()) return {};
    return {it->second.begin(), it->second.end()};
}
std::vector<CapabilityId> DependencyGraph::direct_dependents_of(CapabilityId capability_id) const {
    const auto it = incoming_.find(capability_id);
    if (it == incoming_.end()) return {};
    return {it->second.begin(), it->second.end()};
}
bool DependencyGraph::has_path(CapabilityId from, CapabilityId to) const {
    if (from == to) return true;
    std::set<CapabilityId> visited;
    std::vector<CapabilityId> stack{from};
    while (!stack.empty()) {
        const CapabilityId current = stack.back();
        stack.pop_back();
        if (!visited.insert(current).second) continue;
        const auto it = outgoing_.find(current);
        if (it == outgoing_.end()) continue;
        for (const CapabilityId next : it->second) {
            if (next == to) return true;
            if (!visited.count(next)) stack.push_back(next);
        }
    }
    return false;
}
std::size_t DependencyGraph::edge_count() const noexcept {
    std::size_t count = 0;
    for (const auto& [id, dependencies] : outgoing_) { static_cast<void>(id); count += dependencies.size(); }
    return count;
}
}
