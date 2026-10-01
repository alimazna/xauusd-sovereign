#pragma once

#include "EntityId.h"
#include "KnowledgeId.h"
#include "KnowledgeTransition.h"
#include "Timestamp.h"

namespace xauusd::sovereign {

struct KnowledgeHistoryEntry {
    EntityId             entry_id;
    KnowledgeId          knowledge_id;
    KnowledgeTransition  transition;
    Timestamp            recorded_at;

    KnowledgeHistoryEntry() = default;

    KnowledgeHistoryEntry(
        EntityId entry_id,
        KnowledgeId knowledge_id,
        KnowledgeTransition transition,
        Timestamp recorded_at)
        : entry_id(entry_id),
          knowledge_id(knowledge_id),
          transition(transition),
          recorded_at(recorded_at) {}
};

} // namespace xauusd::sovereign
