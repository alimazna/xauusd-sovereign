#pragma once
#include "IPromotionGate.h"
#include <array>
#include <cstdint>
#include <map>
#include <vector>
namespace xauusd::sovereign {
class PromotionGate final : public IPromotionGate {
public:
    PromotionGate() = default;
    bool submit(const PromotionPackage& package) override;
    bool update_status(const EntityId& package_id, PromotionStatus status) override;
    bool get(const EntityId& package_id, PromotionPackage& out) const override;
    std::vector<PromotionPackage> all() const override;
    std::size_t size() const override;
    void clear();
private:
    std::vector<PromotionPackage> packages_;
    std::map<std::array<std::uint8_t, 16>, std::size_t> index_;
};
}
