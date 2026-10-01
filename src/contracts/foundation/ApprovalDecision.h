#pragma once
#include "ApprovalStatus.h"
#include "EntityId.h"
#include "Timestamp.h"
#include <string>
#include <utility>
namespace xauusd::sovereign {
struct ApprovalDecision {
    EntityId         decision_id;
    EntityId         request_id;
    ApprovalStatus   status;
    std::string      reason;
    Timestamp        decided_at;
    ApprovalDecision() = default;
    ApprovalDecision(EntityId decision_id, EntityId request_id,
                     ApprovalStatus status, std::string reason,
                     Timestamp decided_at)
        : decision_id(decision_id), request_id(request_id),
          status(status), reason(std::move(reason)), decided_at(decided_at) {}
};
}
