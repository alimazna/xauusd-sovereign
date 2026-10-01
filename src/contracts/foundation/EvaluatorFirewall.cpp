#include "EvaluatorFirewall.h"

namespace xauusd::sovereign {

EvaluatorFirewall::Key EvaluatorFirewall::key_of(const EntityId& id) {
    return id.bytes();
}

bool EvaluatorFirewall::register_evaluator(const EvaluatorIdentity& identity) {
    const Key key = key_of(identity.evaluator_id);
    if (index_.find(key) != index_.end()) {
        return false;
    }
    index_.emplace(key, identities_.size());
    identities_.push_back(identity);
    return true;
}

bool EvaluatorFirewall::contains_evaluator(const EntityId& evaluator_id) const {
    return index_.find(key_of(evaluator_id)) != index_.end();
}

bool EvaluatorFirewall::get_evaluator(const EntityId& evaluator_id,
                                      EvaluatorIdentity& out) const {
    const auto it = index_.find(key_of(evaluator_id));
    if (it == index_.end()) {
        return false;
    }
    out = identities_[it->second];
    return true;
}

bool EvaluatorFirewall::freeze(const EntityId& evaluator_id) {
    const Key key = key_of(evaluator_id);
    if (index_.find(key) == index_.end()) {
        return false;
    }
    frozen_.insert(key);
    return true;
}

bool EvaluatorFirewall::is_frozen(const EntityId& evaluator_id) const {
    return frozen_.find(key_of(evaluator_id)) != frozen_.end();
}

std::size_t EvaluatorFirewall::evaluator_count() const {
    return identities_.size();
}

void EvaluatorFirewall::clear() {
    identities_.clear();
    index_.clear();
    frozen_.clear();
}

} // namespace xauusd::sovereign
