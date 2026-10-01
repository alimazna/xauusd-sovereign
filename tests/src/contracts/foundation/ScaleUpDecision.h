#pragma once

#include "EntityId.h"
#include "ScaleUpRequest.h"
#include "Timestamp.h"

#include <string>
#include <utility>

namespace xauusd::sovereign {

struct ScaleUpDecision {
    EntityId        decision_id;
    ScaleUpRequest  request;
    bool            approved;
    std::string     reason;
    Timestamp       decided_at;

    ScaleUpDecision() = default;

    ScaleUpDecision(EntityId decision_id, ScaleUpRequest request,
                    bool approved, std::string reason,
                    Timestamp decided_at)
        : decision_id(decision_id), request(std::move(request)),
          approved(approved), reason(std::move(reason)),
          decided_at(decided_at) {}
};

} // namespace xauusd::sovereign
