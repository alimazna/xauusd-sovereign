#include "OutcomeEngine.h"

namespace xauusd::sovereign {

bool OutcomeEngine::record(const OutcomeRecord& record) {
    const auto key = record.record_id.bytes();

    if (index_.find(key) != index_.end()) {
        return false;
    }

    const std::size_t position = records_.size();
    records_.push_back(record);
    index_.emplace(key, position);

    return true;
}

bool OutcomeEngine::contains(const EntityId& record_id) const {
    return index_.find(record_id.bytes()) != index_.end();
}

bool OutcomeEngine::get(
    const EntityId& record_id,
    OutcomeRecord& out_record) const {
    const auto it = index_.find(record_id.bytes());

    if (it == index_.end()) {
        return false;
    }

    out_record = records_[it->second];
    return true;
}

std::vector<OutcomeRecord> OutcomeEngine::all() const {
    return records_;
}

std::size_t OutcomeEngine::size() const {
    return records_.size();
}

void OutcomeEngine::clear() {
    records_.clear();
    index_.clear();
}

} // namespace xauusd::sovereign
