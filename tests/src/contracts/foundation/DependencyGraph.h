#pragma once
#include "IDependencyGraph.h"
#include "DependencyDescriptor.h"
#include <cstddef>
#include <map>
#include <set>
namespace xauusd::sovereign {
class DependencyGraph final : public IDependencyGraph {
public:
    DependencyGraph() = default;
    bool add_dependency(const DependencyDescriptor& descriptor);
    bool remove_dependency(CapabilityId dependent, CapabilityId dependency);
    std::vector<CapabilityId> direct_dependencies_of(CapabilityId capability_id) const override;
    std::vector<CapabilityId> direct_dependents_of(CapabilityId capability_id) const override;
    bool has_path(CapabilityId from, CapabilityId to) const override;
    std::size_t edge_count() const noexcept;
private:
    std::map<CapabilityId, std::set<CapabilityId>> outgoing_;
    std::map<CapabilityId, std::set<CapabilityId>> incoming_;
};
}
