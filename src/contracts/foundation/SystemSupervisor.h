#pragma once

#include "EntityId.h"
#include "Timestamp.h"
#include "CapabilityId.h"
#include "GuardianStatus.h"
#include "GuardianPolicy.h"
#include "IGuardian.h"
#include "HealthSnapshot.h"
#include "HealthStateEngine.h"
#include "SubsystemIsolationManager.h"
#include "GracefulDegradationManager.h"
#include "PauseResumeManager.h"

#include <optional>
#include <vector>

namespace xauusd::sovereign {

struct SupervisorEvaluation {
    GuardianStatus guardian_status;
    std::vector<CapabilityId> capabilities_to_isolate;
    std::vector<CapabilityId> capabilities_to_pause;
    bool requires_human;
};

class SystemSupervisor {
public:
    SystemSupervisor(
        const IGuardian* guardian,
        SubsystemIsolationManager* isolation,
        const GracefulDegradationManager* degradation,
        PauseResumeManager* pause_resume) noexcept;

    SupervisorEvaluation evaluate(
        CapabilityId failed_capability,
        const GuardianPolicy& policy);

    GuardianStatus last_known_status() const noexcept;

private:
    const IGuardian* guardian_;
    SubsystemIsolationManager* isolation_;
    const GracefulDegradationManager* degradation_;
    PauseResumeManager* pause_resume_;

    GuardianStatus last_status_{GuardianStatus::NORMAL};
};

} // namespace xauusd::sovereign
