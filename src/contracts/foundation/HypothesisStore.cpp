#include "HypothesisStore.h"

namespace xauusd::sovereign {

bool HypothesisStore::add(const Hypothesis& hypothesis) {
    const auto key = hypothesis.hypothesis_id.bytes();
    if (index_.find(key) != index_.end()) {
        return false;
    }

    const std::size_t position = items_.size();
    items_.push_back(hypothesis);
    index_.emplace(key, position);
    return true;
}

bool HypothesisStore::contains(const EntityId& hypothesis_id) const {
    return index_.find(hypothesis_id.bytes()) != index_.end();
}

bool HypothesisStore::get(const EntityId& hypothesis_id, Hypothesis& out) const {
    const auto it = index_.find(hypothesis_id.bytes());
    if (it == index_.end()) {
        return false;
    }

    out = items_[it->second];
    return true;
}

bool HypothesisStore::update_status(const EntityId& hypothesis_id,
                                    HypothesisStatus new_status) {
    const auto it = index_.find(hypothesis_id.bytes());
    if (it == index_.end()) {
        return false;
    }

    items_[it->second].status = new_status;
    return true;
}

std::vector<Hypothesis> HypothesisStore::all() const {
    return items_;
}

std::size_t HypothesisStore::size() const {
    return items_.size();
}

void HypothesisStore::clear() {
    items_.clear();
    index_.clear();
}

} // namespace xauusd::sovereign
