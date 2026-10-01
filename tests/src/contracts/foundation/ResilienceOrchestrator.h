#pragma once

#include "CapabilityId.h"
#include "CapabilityDescriptor.h"
#include "DependencyDescriptor.h"
#include "HealthSnapshot.h"
#include "FreshnessState.h"
#include "DegradationImpact.h"
#include "CriticalityPolicy.h"
#include "GuardianStatus.h"
#include "GuardianPolicy.h"
#include "IGuardian.h"
#include "CapabilityRegistry.h"
#include "DependencyGraph.h"
#include "HealthStateEngine.h"
#include "DataFreshnessMonitor.h"
#include "SubsystemIsolationManager.h"
#include "GracefulDegradationManager.h"
#include "RecoveryManager.h"
#include "PauseResumeManager.h"
#include "SystemSupervisor.h"
#include "StaleCandleDetector.h"

#include <cstdint>
#include <vector>

namespace xauusd::sovereign {

class ResilienceOrchestrator {
public:
    explicit ResilienceOrchestrator(const IGuardian* guardian) noexcept;

    bool register_capability(const CapabilityDescriptor& descriptor);
    bool register_dependency(const DependencyDescriptor& dependency);

    bool record_service_state(
        const EntityId& service_id,
        ServiceState service_state,
        FreshnessState freshness);
    void observe_data(CapabilityId capability_id, Timestamp observed_at);

    SupervisorEvaluation evaluate_failure(
        CapabilityId failed_capability,
        const GuardianPolicy& policy);

    RecoveryAction recommend_recovery(
        GuardianStatus status,
        const CriticalityPolicy& policy) const;

    FreshnessState evaluate_freshness(
        CapabilityId capability_id,
        Timestamp now,
        std::int64_t fresh_threshold_us,
        std::int64_t aging_threshold_us) const;

    bool is_candle_stale(Timestamp candle_time, Timestamp now) const;

    bool pause(CapabilityId capability_id, Timestamp at);
    bool resume(CapabilityId capability_id, Timestamp at);

    const CapabilityRegistry& registry() const noexcept;
    const DependencyGraph& graph() const noexcept;
    const HealthStateEngine& health() const noexcept;
    const PauseResumeManager& pause_resume() const noexcept;

private:
    CapabilityRegistry registry_;
    DependencyGraph graph_;
    HealthStateEngine health_;
    DataFreshnessMonitor freshness_;
    SubsystemIsolationManager isolation_;
    GracefulDegradationManager degradation_;
    RecoveryManager recovery_;
    PauseResumeManager pause_resume_;
    SystemSupervisor supervisor_;
    StaleCandleDetector stale_detector_;

    void rebind_internal_components() noexcept;
};

} // namespace xauusd::sovereign
