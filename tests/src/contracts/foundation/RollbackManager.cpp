#include "RollbackManager.h"
namespace xauusd::sovereign {
bool RollbackManager::submit(const RollbackRequest& request) {
    const auto key = request.request_id.bytes();
    if (index_.find(key) != index_.end()) {
        return false;
    }
    requests_.push_back(request);
    index_.emplace(key, requests_.size() - 1U);
    return true;
}

bool RollbackManager::record_result(const RollbackResult& result) {
    const auto key = result.request.request_id.bytes();
    if (index_.find(key) == index_.end()) {
        return false;
    }
    results_.push_back(result);
    return true;
}

bool RollbackManager::contains(const EntityId& request_id) const {
    return index_.find(request_id.bytes()) != index_.end();
}

bool RollbackManager::get_request(const EntityId& request_id, RollbackRequest& out) const {
    const auto it = index_.find(request_id.bytes());
    if (it == index_.end()) {
        return false;
    }
    out = requests_[it->second];
    return true;
}

std::vector<RollbackResult> RollbackManager::all_results() const { return results_; }
std::size_t RollbackManager::request_count() const { return requests_.size(); }

void RollbackManager::clear() {
    requests_.clear();
    index_.clear();
    results_.clear();
}
} // namespace xauusd::sovereign
