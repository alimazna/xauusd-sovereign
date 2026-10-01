#pragma once
#include "EntityId.h"
#include "Version.h"
#include "CapabilityId.h"
#include <string>
#include <utility>
namespace xauusd::sovereign {
struct ServiceDescriptor {
    EntityId service_id{};
    std::string service_name{};
    Version service_version{};
    CapabilityId primary_capability{};
    ServiceDescriptor() = default;
    ServiceDescriptor(const EntityId& id, std::string name, const Version& version, CapabilityId capability)
        : service_id(id), service_name(std::move(name)), service_version(version), primary_capability(capability) {}
};
}
