#include "ResilienceOrchestrator.h"

namespace xauusd::sovereign {

ResilienceOrchestrator::ResilienceOrchestrator(
    const IGuardian* guardian) noexcept
    : isolation_(&graph_),
      degradation_(&graph_, &registry_),
      supervisor_(guardian, &isolation_, &degradation_, &pause_resume_) {}

bool ResilienceOrchestrator::register_capability(
    const CapabilityDescriptor& descriptor) {
    const bool registered = registry_.register_capability(descriptor);
    rebind_internal_components();
    return registered;
}

bool ResilienceOrchestrator::register_dependency(
    const DependencyDescriptor& dependency) {
    const bool registered = graph_.add_dependency(dependency);
    rebind_internal_components();
    return registered;
}

bool ResilienceOrchestrator::record_service_state(
    const EntityId& service_id,
    ServiceState service_state,
    FreshnessState freshness) {
    return health_.record_state(service_id, service_state, freshness);
}

void ResilienceOrchestrator::observe_data(
    CapabilityId capability_id,
    Timestamp observed_at) {
    freshness_.observe(capability_id, observed_at);
}

SupervisorEvaluation ResilienceOrchestrator::evaluate_failure(
    CapabilityId failed_capability,
    const GuardianPolicy& policy) {
    return supervisor_.evaluate(failed_capability, policy);
}

RecoveryAction ResilienceOrchestrator::recommend_recovery(
    GuardianStatus status,
    const CriticalityPolicy& policy) const {
    return recovery_.recommend(status, policy);
}

FreshnessState ResilienceOrchestrator::evaluate_freshness(
    CapabilityId capability_id,
    Timestamp now,
    std::int64_t fresh_threshold_us,
    std::int64_t aging_threshold_us) const {
    return freshness_.evaluate(
        capability_id,
        now,
        fresh_threshold_us,
        aging_threshold_us);
}

bool ResilienceOrchestrator::is_candle_stale(
    Timestamp candle_time,
    Timestamp now) const {
    return stale_detector_.is_stale(candle_time, now);
}

bool ResilienceOrchestrator::pause(
    CapabilityId capability_id,
    Timestamp at) {
    return pause_resume_.pause(capability_id, at);
}

bool ResilienceOrchestrator::resume(
    CapabilityId capability_id,
    Timestamp at) {
    return pause_resume_.resume(capability_id, at);
}

const CapabilityRegistry& ResilienceOrchestrator::registry() const noexcept {
    return registry_;
}

const DependencyGraph& ResilienceOrchestrator::graph() const noexcept {
    return graph_;
}

const HealthStateEngine& ResilienceOrchestrator::health() const noexcept {
    return health_;
}

const PauseResumeManager& ResilienceOrchestrator::pause_resume() const noexcept {
    return pause_resume_;
}

void ResilienceOrchestrator::rebind_internal_components() noexcept {
}

} // namespace xauusd::sovereign
