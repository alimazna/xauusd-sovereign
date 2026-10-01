#pragma once
#include "EntityId.h"
#include "RecoveryPhase.h"
#include "Timestamp.h"
#include <string>
#include <utility>
namespace xauusd::sovereign {
struct RecoveryPlan {
    EntityId      plan_id;
    EntityId      checkpoint_id;
    RecoveryPhase starting_phase;
    std::string   reason;
    Timestamp     created_at;
    RecoveryPlan() = default;
    RecoveryPlan(EntityId plan_id, EntityId checkpoint_id,
                 RecoveryPhase starting_phase, std::string reason,
                 Timestamp created_at)
        : plan_id(plan_id), checkpoint_id(checkpoint_id), starting_phase(starting_phase),
          reason(std::move(reason)), created_at(created_at) {}
};
}
