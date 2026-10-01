#include "ExperimentLedger.h"

namespace xauusd::sovereign {

bool ExperimentLedger::add(const Experiment& experiment) {
    const auto key = experiment.experiment_id.bytes();
    if (index_.find(key) != index_.end()) {
        return false;
    }
    if (has_fingerprint(experiment.fingerprint)) {
        return false;
    }

    const std::size_t position = items_.size();
    items_.push_back(experiment);
    index_.emplace(key, position);
    return true;
}

bool ExperimentLedger::record_result(const ExperimentResult& result) {
    if (!contains(result.experiment_id)) {
        return false;
    }

    results_.push_back(result);
    return true;
}

bool ExperimentLedger::contains(const EntityId& experiment_id) const {
    return index_.find(experiment_id.bytes()) != index_.end();
}

bool ExperimentLedger::get(const EntityId& experiment_id, Experiment& out) const {
    const auto it = index_.find(experiment_id.bytes());
    if (it == index_.end()) {
        return false;
    }

    out = items_[it->second];
    return true;
}

bool ExperimentLedger::has_fingerprint(const ExperimentFingerprint& fp) const {
    for (const Experiment& experiment : items_) {
        if (experiment.fingerprint.digest == fp.digest) {
            return true;
        }
    }
    return false;
}

std::vector<Experiment> ExperimentLedger::all() const {
    return items_;
}

std::vector<ExperimentResult> ExperimentLedger::results_for(
    const EntityId& experiment_id) const {
    std::vector<ExperimentResult> matching;
    for (const ExperimentResult& result : results_) {
        if (result.experiment_id == experiment_id) {
            matching.push_back(result);
        }
    }
    return matching;
}

std::size_t ExperimentLedger::size() const {
    return items_.size();
}

void ExperimentLedger::clear() {
    items_.clear();
    index_.clear();
    results_.clear();
}

} // namespace xauusd::sovereign
