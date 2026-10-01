#include "PromotionGate.h"
namespace xauusd::sovereign {
bool PromotionGate::submit(const PromotionPackage& package) {
    const auto key = package.package_id.bytes();
    if (index_.find(key) != index_.end()) {
        return false;
    }
    packages_.push_back(package);
    index_.emplace(key, packages_.size() - 1U);
    return true;
}

bool PromotionGate::update_status(const EntityId& package_id, PromotionStatus status) {
    const auto it = index_.find(package_id.bytes());
    if (it == index_.end()) {
        return false;
    }
    packages_[it->second].status = status;
    return true;
}

bool PromotionGate::get(const EntityId& package_id, PromotionPackage& out) const {
    const auto it = index_.find(package_id.bytes());
    if (it == index_.end()) {
        return false;
    }
    out = packages_[it->second];
    return true;
}

std::vector<PromotionPackage> PromotionGate::all() const { return packages_; }
std::size_t PromotionGate::size() const { return packages_.size(); }

void PromotionGate::clear() {
    packages_.clear();
    index_.clear();
}
} // namespace xauusd::sovereign
