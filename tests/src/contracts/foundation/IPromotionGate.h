#pragma once
#include "EntityId.h"
#include "PromotionPackage.h"
#include "PromotionStatus.h"
#include <cstddef>
#include <vector>
namespace xauusd::sovereign {
class IPromotionGate {
public:
    virtual ~IPromotionGate() = default;
    virtual bool submit(const PromotionPackage& package) = 0;
    virtual bool update_status(const EntityId& package_id, PromotionStatus status) = 0;
    virtual bool get(const EntityId& package_id, PromotionPackage& out) const = 0;
    virtual std::vector<PromotionPackage> all() const = 0;
    virtual std::size_t size() const = 0;
};
}
