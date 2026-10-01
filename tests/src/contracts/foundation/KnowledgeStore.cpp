#include "KnowledgeStore.h"

namespace xauusd::sovereign {

bool KnowledgeStore::add(const KnowledgeObject& object) {
    const auto key = object.knowledge_id.entity_id().bytes();

    if (index_.find(key) != index_.end()) {
        return false;
    }

    index_.emplace(key, objects_.size());
    objects_.push_back(object);
    return true;
}

bool KnowledgeStore::contains(const KnowledgeId& id) const {
    return index_.find(id.entity_id().bytes()) != index_.end();
}

bool KnowledgeStore::get(
    const KnowledgeId& id,
    KnowledgeObject& out_object) const {
    const auto it = index_.find(id.entity_id().bytes());
    if (it == index_.end()) {
        return false;
    }

    out_object = objects_[it->second];
    return true;
}

bool KnowledgeStore::update(const KnowledgeObject& object) {
    const auto key = object.knowledge_id.entity_id().bytes();
    const auto it = index_.find(key);
    if (it == index_.end()) {
        return false;
    }

    objects_[it->second] = object;
    return true;
}

std::vector<KnowledgeObject> KnowledgeStore::all() const {
    return objects_;
}

std::size_t KnowledgeStore::size() const {
    return objects_.size();
}

void KnowledgeStore::clear() {
    objects_.clear();
    index_.clear();
}

} // namespace xauusd::sovereign
