#include "HealthStateEngine.h"
namespace xauusd::sovereign {
HealthSnapshot HealthStateEngine::current_snapshot(const EntityId& service_id) const {
    const auto it = snapshots_.find(service_id.bytes());
    return it == snapshots_.end() ? HealthSnapshot{} : it->second;
}
bool HealthStateEngine::record_state(const EntityId& service_id, ServiceState service_state, FreshnessState freshness) {
    HealthSnapshot snapshot{};
    snapshot.service_id = service_id;
    snapshot.service_state = service_state;
    snapshot.freshness = freshness;
    snapshots_[service_id.bytes()] = snapshot;
    return true;
}
std::size_t HealthStateEngine::tracked_services() const noexcept { return snapshots_.size(); }
}
