#pragma once
#include "CapabilityId.h"
#include "CapabilityDescriptor.h"
#include <vector>
namespace xauusd::sovereign {
class ICapabilityRegistry {
public:
    virtual ~ICapabilityRegistry() = default;
    virtual bool contains(CapabilityId) const = 0;
    virtual bool get_descriptor(CapabilityId, CapabilityDescriptor&) const = 0;
    virtual std::vector<CapabilityDescriptor> all() const = 0;
};
}
