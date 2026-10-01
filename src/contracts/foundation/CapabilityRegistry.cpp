#include "CapabilityRegistry.h"
namespace xauusd::sovereign {
bool CapabilityRegistry::register_capability(const CapabilityDescriptor& descriptor) {
    const auto [it, inserted] = descriptors_.insert_or_assign(descriptor.capability_id, descriptor);
    static_cast<void>(it);
    return inserted;
}
bool CapabilityRegistry::contains(CapabilityId capability_id) const { return descriptors_.find(capability_id) != descriptors_.end(); }
bool CapabilityRegistry::get_descriptor(CapabilityId capability_id, CapabilityDescriptor& out) const {
    const auto it = descriptors_.find(capability_id);
    if (it == descriptors_.end()) return false;
    out = it->second;
    return true;
}
std::vector<CapabilityDescriptor> CapabilityRegistry::all() const {
    std::vector<CapabilityDescriptor> result;
    result.reserve(descriptors_.size());
    for (const auto& [id, descriptor] : descriptors_) { static_cast<void>(id); result.push_back(descriptor); }
    return result;
}
std::size_t CapabilityRegistry::size() const noexcept { return descriptors_.size(); }
}
