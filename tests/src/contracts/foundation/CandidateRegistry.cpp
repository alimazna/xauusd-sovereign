#include "CandidateRegistry.h"

namespace xauusd::sovereign {

bool CandidateRegistry::add(const Candidate& candidate) {
    const auto key = candidate.candidate_id.bytes();
    if (index_.find(key) != index_.end()) {
        return false;
    }

    items_.push_back(candidate);
    index_.emplace(key, items_.size() - 1U);
    return true;
}

bool CandidateRegistry::contains(const EntityId& candidate_id) const {
    return index_.find(candidate_id.bytes()) != index_.end();
}

bool CandidateRegistry::get(const EntityId& candidate_id, Candidate& out) const {
    const auto it = index_.find(candidate_id.bytes());
    if (it == index_.end()) {
        return false;
    }

    out = items_[it->second];
    return true;
}

bool CandidateRegistry::update_status(const EntityId& candidate_id,
                                      CandidateStatus new_status) {
    const auto it = index_.find(candidate_id.bytes());
    if (it == index_.end()) {
        return false;
    }

    items_[it->second].status = new_status;
    return true;
}

std::vector<Candidate> CandidateRegistry::all() const {
    return items_;
}

std::vector<Candidate> CandidateRegistry::children_of(const EntityId& parent_id) const {
    std::vector<Candidate> children;
    for (const auto& candidate : items_) {
        if (candidate.parent_candidate_id == parent_id) {
            children.push_back(candidate);
        }
    }
    return children;
}

std::size_t CandidateRegistry::size() const {
    return items_.size();
}

void CandidateRegistry::clear() {
    items_.clear();
    index_.clear();
}

} // namespace xauusd::sovereign
