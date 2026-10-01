#pragma once
#include "ICapabilityRegistry.h"
#include <cstddef>
#include <map>
namespace xauusd::sovereign {
class CapabilityRegistry final : public ICapabilityRegistry {
public:
    CapabilityRegistry() = default;
    bool register_capability(const CapabilityDescriptor& descriptor);
    bool contains(CapabilityId capability_id) const override;
    bool get_descriptor(CapabilityId capability_id, CapabilityDescriptor& out) const override;
    std::vector<CapabilityDescriptor> all() const override;
    std::size_t size() const noexcept;
private:
    std::map<CapabilityId, CapabilityDescriptor> descriptors_;
};
}
