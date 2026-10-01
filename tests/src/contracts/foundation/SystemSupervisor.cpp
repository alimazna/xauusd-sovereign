#include "SystemSupervisor.h"

namespace xauusd::sovereign {

SystemSupervisor::SystemSupervisor(
    const IGuardian* guardian,
    SubsystemIsolationManager* isolation,
    const GracefulDegradationManager* degradation,
    PauseResumeManager* pause_resume) noexcept
    : guardian_(guardian),
      isolation_(isolation),
      degradation_(degradation),
      pause_resume_(pause_resume) {}

SupervisorEvaluation SystemSupervisor::evaluate(
    CapabilityId failed_capability,
    const GuardianPolicy& policy) {
    SupervisorEvaluation evaluation{
        last_status_,
        {},
        {},
        false
    };

    if (guardian_ != nullptr) {
        evaluation.guardian_status = guardian_->status();
    }

    if (isolation_ != nullptr) {
        evaluation.capabilities_to_isolate = isolation_->isolate(failed_capability);
    }

    if (pause_resume_ != nullptr) {
        for (const CapabilityId capability_id : evaluation.capabilities_to_isolate) {
            if (pause_resume_->pause(capability_id, Timestamp{})) {
                evaluation.capabilities_to_pause.push_back(capability_id);
            }
        }
    }

    if (degradation_ != nullptr) {
        static_cast<void>(degradation_->evaluate_impact(failed_capability));
    }

    evaluation.requires_human =
        evaluation.guardian_status == GuardianStatus::SAFE_MODE ||
        evaluation.guardian_status == GuardianStatus::HALT ||
        policy.requires_human_intervention;

    last_status_ = evaluation.guardian_status;
    return evaluation;
}

GuardianStatus SystemSupervisor::last_known_status() const noexcept {
    return last_status_;
}

} // namespace xauusd::sovereign
