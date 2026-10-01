#include "RecoveryManager.h"
namespace xauusd::sovereign {
RecoveryAction RecoveryManager::recommend(GuardianStatus current_status, const CriticalityPolicy& policy) const {
    if (policy.requires_human_intervention) return RecoveryAction::MANUAL_INTERVENTION_REQUIRED;
    switch (current_status) {
    case GuardianStatus::NORMAL: return RecoveryAction::NONE;
    case GuardianStatus::OBSERVE: return RecoveryAction::NONE;
    case GuardianStatus::WARN: return RecoveryAction::NONE;
    case GuardianStatus::DEGRADE: return RecoveryAction::ENTER_DEGRADED;
    case GuardianStatus::SAFE_MODE: return RecoveryAction::PAUSE;
    case GuardianStatus::HALT: return RecoveryAction::HALT;
    }
    return RecoveryAction::NONE;
}
bool RecoveryManager::requires_human(GuardianStatus current_status, const CriticalityPolicy& policy) const { return policy.requires_human_intervention || current_status == GuardianStatus::SAFE_MODE || current_status == GuardianStatus::HALT; }
}
