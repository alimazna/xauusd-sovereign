#pragma once
#include "GuardianStatus.h"
#include "RecoveryAction.h"
#include "CriticalityPolicy.h"
namespace xauusd::sovereign {
class RecoveryManager {
public:
    RecoveryManager() = default;
    RecoveryAction recommend(GuardianStatus current_status, const CriticalityPolicy& policy) const;
    bool requires_human(GuardianStatus current_status, const CriticalityPolicy& policy) const;
};
}
