#pragma once
#include "EntityId.h"
#include "KnownGoodVersion.h"
#include "Version.h"
#include <cstddef>
#include <vector>
namespace xauusd::sovereign {
class IKnownGoodRegistry {
public:
    virtual ~IKnownGoodRegistry() = default;
    virtual bool register_version(const KnownGoodVersion& entry) = 0;
    virtual bool contains_version(Version version) const = 0;
    virtual bool get_version(Version version, KnownGoodVersion& out) const = 0;
    virtual KnownGoodVersion latest() const = 0;
    virtual std::vector<KnownGoodVersion> all() const = 0;
    virtual std::size_t size() const = 0;
};
}
