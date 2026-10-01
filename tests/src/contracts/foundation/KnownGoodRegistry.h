#pragma once
#include "IKnownGoodRegistry.h"
#include <cstddef>
#include <cstdint>
#include <map>
#include <vector>
namespace xauusd::sovereign {
class KnownGoodRegistry final : public IKnownGoodRegistry {
public:
    KnownGoodRegistry() = default;
    bool register_version(const KnownGoodVersion& entry) override;
    bool contains_version(Version version) const override;
    bool get_version(Version version, KnownGoodVersion& out) const override;
    KnownGoodVersion latest() const override;
    std::vector<KnownGoodVersion> all() const override;
    std::size_t size() const override;
    void clear();
private:
    std::vector<KnownGoodVersion> entries_;
    std::map<std::uint64_t, std::size_t> index_;
};
}
