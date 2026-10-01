#pragma once

#include "EntityId.h"
#include "Timestamp.h"

#include <string>
#include <utility>

namespace xauusd::sovereign {

struct LiveDecision {
    EntityId    decision_id;
    EntityId    session_id;
    EntityId    candidate_id;
    bool        accepted;
    std::string reason;
    Timestamp   decided_at;

    LiveDecision() = default;

    LiveDecision(EntityId decision_id, EntityId session_id,
                 EntityId candidate_id, bool accepted,
                 std::string reason, Timestamp decided_at)
        : decision_id(decision_id), session_id(session_id),
          candidate_id(candidate_id), accepted(accepted),
          reason(std::move(reason)), decided_at(decided_at) {}
};

} // namespace xauusd::sovereign
