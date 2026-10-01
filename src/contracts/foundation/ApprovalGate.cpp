#include "ApprovalGate.h"
namespace xauusd::sovereign {
bool ApprovalGate::submit(const ApprovalRequest& request) {
    const auto key = request.request_id.bytes();
    if (request_index_.find(key) != request_index_.end()) {
        return false;
    }
    requests_.push_back(request);
    request_index_.emplace(key, requests_.size() - 1U);
    return true;
}

bool ApprovalGate::decide(const ApprovalDecision& decision) {
    const auto key = decision.request_id.bytes();
    if (request_index_.find(key) == request_index_.end() || decided_.find(key) != decided_.end()) {
        return false;
    }
    const auto index = request_index_.at(key);
    records_.emplace_back(decision.decision_id, requests_[index], decision);
    decided_.insert(key);
    return true;
}

bool ApprovalGate::get_request(const EntityId& request_id, ApprovalRequest& out) const {
    const auto it = request_index_.find(request_id.bytes());
    if (it == request_index_.end()) {
        return false;
    }
    out = requests_[it->second];
    return true;
}

std::vector<ApprovalRecord> ApprovalGate::all_records() const { return records_; }

std::size_t ApprovalGate::pending_count() const { return requests_.size() - decided_.size(); }

std::size_t ApprovalGate::record_count() const { return records_.size(); }

void ApprovalGate::clear() {
    requests_.clear();
    request_index_.clear();
    records_.clear();
    decided_.clear();
}
} // namespace xauusd::sovereign
