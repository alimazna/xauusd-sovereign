#pragma once

#include "GuardianStatus.h"
#include "GuardianPolicy.h"
#include "RecoveryAction.h"

namespace xauusd::sovereign {

class IGuardian {
public:
    virtual ~IGuardian() = default;

    virtual GuardianStatus status() const = 0;

    virtual bool evaluate(
        const GuardianPolicy& policy) = 0;

    virtual RecoveryAction recommend_recovery(
        const GuardianStatus& status) const = 0;
};

} // namespace xauusd::sovereign
