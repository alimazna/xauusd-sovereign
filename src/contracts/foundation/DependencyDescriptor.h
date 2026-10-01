#pragma once
#include "CapabilityId.h"
namespace xauusd::sovereign {
struct DependencyDescriptor {
    CapabilityId dependent{};
    CapabilityId dependency{};
    bool mandatory{false};
    DependencyDescriptor() = default;
    DependencyDescriptor(CapabilityId dependent_id, CapabilityId dependency_id, bool is_mandatory)
        : dependent(dependent_id), dependency(dependency_id), mandatory(is_mandatory) {}
};
}
