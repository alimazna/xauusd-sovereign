#pragma once

#include "IGuardian.h"
#include "GuardianStatus.h"
#include "GuardianPolicy.h"
#include "RecoveryAction.h"

namespace xauusd::sovereign {

class Guardian final : public IGuardian {
public:
    Guardian() = default;

    explicit Guardian(const GuardianPolicy& initial_policy)
        : policy_(initial_policy) {}

    GuardianStatus status() const noexcept override {
        return policy_.active_status;
    }

    bool evaluate(const GuardianPolicy& policy) override {
        policy_ = policy;
        return policy_.active_status == GuardianStatus::NORMAL ||
               policy_.active_status == GuardianStatus::OBSERVE;
    }

    RecoveryAction recommend_recovery(
        const GuardianStatus& status) const override {
        switch (status) {
            case GuardianStatus::NORMAL:
                return RecoveryAction::NONE;
            case GuardianStatus::OBSERVE:
                return RecoveryAction::NONE;
            case GuardianStatus::WARN:
                return RecoveryAction::NONE;
            case GuardianStatus::DEGRADE:
                return RecoveryAction::ENTER_DEGRADED;
            case GuardianStatus::SAFE_MODE:
                return RecoveryAction::PAUSE;
            case GuardianStatus::HALT:
                return RecoveryAction::HALT;
        }

        return RecoveryAction::HALT;
    }

private:
    GuardianPolicy policy_{};
};

} // namespace xauusd::sovereign
