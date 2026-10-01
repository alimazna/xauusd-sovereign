#include "ContaminationTracker.h"

namespace xauusd::sovereign {

ContaminationTracker::Key ContaminationTracker::key_of(const EntityId& id) {
    return id.bytes();
}

bool ContaminationTracker::mark(const EntityId& candidate_id,
                                ContaminationState state) {
    states_[key_of(candidate_id)] = state;
    return true;
}

bool ContaminationTracker::get(const EntityId& candidate_id,
                               ContaminationState& out) const {
    const auto it = states_.find(key_of(candidate_id));
    if (it == states_.end()) {
        return false;
    }
    out = it->second;
    return true;
}

bool ContaminationTracker::contains(const EntityId& candidate_id) const {
    return states_.find(key_of(candidate_id)) != states_.end();
}

std::size_t ContaminationTracker::size() const {
    return states_.size();
}

void ContaminationTracker::clear() {
    states_.clear();
}

} // namespace xauusd::sovereign
