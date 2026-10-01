#include "IncidentTracker.h"
namespace xauusd::sovereign {
bool IncidentTracker::record(const Incident& incident) {
    const auto key = incident.incident_id.bytes();
    if (index_.find(key) != index_.end()) {
        return false;
    }
    incidents_.push_back(incident);
    index_.emplace(key, incidents_.size() - 1U);
    return true;
}

bool IncidentTracker::contains(const EntityId& incident_id) const {
    return index_.find(incident_id.bytes()) != index_.end();
}

bool IncidentTracker::get(const EntityId& incident_id, Incident& out) const {
    const auto it = index_.find(incident_id.bytes());
    if (it == index_.end()) {
        return false;
    }
    out = incidents_[it->second];
    return true;
}

bool IncidentTracker::resolve(const EntityId& incident_id) {
    const auto it = index_.find(incident_id.bytes());
    if (it == index_.end()) {
        return false;
    }
    incidents_[it->second].resolved = true;
    return true;
}

std::vector<Incident> IncidentTracker::all() const { return incidents_; }

std::vector<Incident> IncidentTracker::unresolved() const {
    std::vector<Incident> result;
    for (const auto& incident : incidents_) {
        if (!incident.resolved) {
            result.push_back(incident);
        }
    }
    return result;
}

std::size_t IncidentTracker::size() const { return incidents_.size(); }

void IncidentTracker::clear() {
    incidents_.clear();
    index_.clear();
}
} // namespace xauusd::sovereign
