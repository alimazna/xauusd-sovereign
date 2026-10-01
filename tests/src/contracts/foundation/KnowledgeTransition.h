#pragma once

#include "KnowledgeId.h"
#include "KnowledgeStatus.h"
#include "Timestamp.h"

#include <string>
#include <utility>

namespace xauusd::sovereign {

struct KnowledgeTransition {
    KnowledgeId      knowledge_id;
    KnowledgeStatus  from_status;
    KnowledgeStatus  to_status;
    Timestamp        transition_time;
    std::string      reason;

    KnowledgeTransition() = default;

    KnowledgeTransition(
        KnowledgeId knowledge_id,
        KnowledgeStatus from_status,
        KnowledgeStatus to_status,
        Timestamp transition_time,
        std::string reason)
        : knowledge_id(knowledge_id),
          from_status(from_status),
          to_status(to_status),
          transition_time(transition_time),
          reason(std::move(reason)) {}
};

} // namespace xauusd::sovereign
