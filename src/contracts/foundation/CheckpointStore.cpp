#include "CheckpointStore.h"
namespace xauusd::sovereign {
bool CheckpointStore::store(const Checkpoint& checkpoint) {
    const auto key = checkpoint.metadata.checkpoint_id.bytes();
    if (index_.find(key) != index_.end()) {
        return false;
    }
    items_.push_back(checkpoint);
    index_.emplace(key, items_.size() - 1U);
    return true;
}

bool CheckpointStore::contains(const EntityId& checkpoint_id) const {
    return index_.find(checkpoint_id.bytes()) != index_.end();
}

bool CheckpointStore::get(const EntityId& checkpoint_id, Checkpoint& out) const {
    const auto it = index_.find(checkpoint_id.bytes());
    if (it == index_.end()) {
        return false;
    }
    out = items_[it->second];
    return true;
}

bool CheckpointStore::update_status(const EntityId& checkpoint_id, CheckpointStatus status) {
    const auto it = index_.find(checkpoint_id.bytes());
    if (it == index_.end()) {
        return false;
    }
    items_[it->second].status = status;
    return true;
}

std::vector<Checkpoint> CheckpointStore::all() const { return items_; }
std::size_t CheckpointStore::size() const { return items_.size(); }

void CheckpointStore::clear() {
    items_.clear();
    index_.clear();
}
} // namespace xauusd::sovereign
