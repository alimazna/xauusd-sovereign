#include "FailureMemory.h"

namespace xauusd::sovereign {

bool FailureMemory::record(const FailureMemoryEntry& entry) {
    const auto key = entry.entry_id.bytes();
    if (entries_index_.find(key) != entries_index_.end()) {
        return false;
    }

    entries_index_.emplace(key, entries_.size());
    entries_.push_back(entry);

    for (const auto& hypothesis : entry.hypotheses) {
        add_hypothesis(hypothesis);
    }

    return true;
}

bool FailureMemory::contains(const EntityId& entry_id) const {
    return entries_index_.find(entry_id.bytes()) != entries_index_.end();
}

bool FailureMemory::get(
    const EntityId& entry_id,
    FailureMemoryEntry& out_entry) const {
    const auto it = entries_index_.find(entry_id.bytes());
    if (it == entries_index_.end()) {
        return false;
    }

    out_entry = entries_[it->second];
    return true;
}

std::vector<FailureMemoryEntry> FailureMemory::all() const {
    return entries_;
}

std::size_t FailureMemory::size() const {
    return entries_.size();
}

bool FailureMemory::add_hypothesis(const RCAHypothesis& h) {
    const auto key = h.hypothesis_id.bytes();
    if (hypotheses_index_.find(key) != hypotheses_index_.end()) {
        return false;
    }

    hypotheses_index_.emplace(key, hypotheses_.size());
    hypotheses_.push_back(h);
    return true;
}

bool FailureMemory::confirm_hypothesis(const EntityId& hypothesis_id) {
    const auto it = hypotheses_index_.find(hypothesis_id.bytes());
    if (it == hypotheses_index_.end()) {
        return false;
    }

    auto& hypothesis = hypotheses_[it->second];
    hypothesis.confirmed = true;
    hypothesis.rejected = false;
    return true;
}

bool FailureMemory::reject_hypothesis(const EntityId& hypothesis_id) {
    const auto it = hypotheses_index_.find(hypothesis_id.bytes());
    if (it == hypotheses_index_.end()) {
        return false;
    }

    auto& hypothesis = hypotheses_[it->second];
    hypothesis.confirmed = false;
    hypothesis.rejected = true;
    return true;
}

std::vector<RCAHypothesis> FailureMemory::hypotheses_for(
    const EntityId& failure_id) const {
    std::vector<RCAHypothesis> result;

    for (const auto& hypothesis : hypotheses_) {
        if (hypothesis.failure_pattern_id == failure_id) {
            result.push_back(hypothesis);
        }
    }

    return result;
}

void FailureMemory::clear() {
    entries_.clear();
    entries_index_.clear();
    hypotheses_.clear();
    hypotheses_index_.clear();
}

} // namespace xauusd::sovereign
