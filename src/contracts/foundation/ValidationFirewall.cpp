#include "ValidationFirewall.h"

namespace xauusd::sovereign {

ValidationFirewall::Key ValidationFirewall::key_of(const EntityId& id) {
    return id.bytes();
}

bool ValidationFirewall::register_protocol(const ValidationProtocol& protocol) {
    const Key key = key_of(protocol.protocol_id);
    if (protocol_index_.find(key) != protocol_index_.end()) {
        return false;
    }
    protocol_index_.emplace(key, protocols_.size());
    protocols_.push_back(protocol);
    return true;
}

bool ValidationFirewall::get_protocol(const EntityId& protocol_id,
                                      ValidationProtocol& out) const {
    const auto it = protocol_index_.find(key_of(protocol_id));
    if (it == protocol_index_.end()) {
        return false;
    }
    out = protocols_[it->second];
    return true;
}

bool ValidationFirewall::record_run(const ValidationRun& run) {
    const Key key = key_of(run.run_id);
    if (run_index_.find(key) != run_index_.end()) {
        return false;
    }
    run_index_.emplace(key, runs_.size());
    runs_.push_back(run);
    return true;
}

bool ValidationFirewall::record_result(const ValidationResult& result) {
    if (run_index_.find(key_of(result.run_id)) == run_index_.end()) {
        return false;
    }
    results_.push_back(result);
    return true;
}

bool ValidationFirewall::all_passed(const EntityId& run_id) const {
    bool saw_result = false;
    for (const auto& result : results_) {
        if (result.run_id == run_id) {
            saw_result = true;
            if (!result.passed) {
                return false;
            }
        }
    }
    return saw_result;
}

std::vector<ValidationResult> ValidationFirewall::results_for(
    const EntityId& run_id) const {
    std::vector<ValidationResult> out;
    for (const auto& result : results_) {
        if (result.run_id == run_id) {
            out.push_back(result);
        }
    }
    return out;
}

std::size_t ValidationFirewall::protocol_count() const {
    return protocols_.size();
}

std::size_t ValidationFirewall::run_count() const {
    return runs_.size();
}

void ValidationFirewall::clear() {
    protocols_.clear();
    protocol_index_.clear();
    runs_.clear();
    run_index_.clear();
    results_.clear();
}

} // namespace xauusd::sovereign
