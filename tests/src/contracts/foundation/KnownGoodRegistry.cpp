#include "KnownGoodRegistry.h"
#include <iterator>
namespace xauusd::sovereign {
bool KnownGoodRegistry::register_version(const KnownGoodVersion& entry) {
    const auto key = entry.version.value();
    if (index_.find(key) != index_.end()) {
        return false;
    }
    entries_.push_back(entry);
    index_.emplace(key, entries_.size() - 1U);
    return true;
}

bool KnownGoodRegistry::contains_version(Version version) const {
    return index_.find(version.value()) != index_.end();
}

bool KnownGoodRegistry::get_version(Version version, KnownGoodVersion& out) const {
    const auto it = index_.find(version.value());
    if (it == index_.end()) {
        return false;
    }
    out = entries_[it->second];
    return true;
}

KnownGoodVersion KnownGoodRegistry::latest() const {
    if (index_.empty()) {
        return KnownGoodVersion{};
    }
    const auto it = std::prev(index_.end());
    return entries_[it->second];
}

std::vector<KnownGoodVersion> KnownGoodRegistry::all() const { return entries_; }
std::size_t KnownGoodRegistry::size() const { return entries_.size(); }

void KnownGoodRegistry::clear() {
    entries_.clear();
    index_.clear();
}
} // namespace xauusd::sovereign
