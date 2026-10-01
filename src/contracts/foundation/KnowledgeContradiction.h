#pragma once

#include "EntityId.h"
#include "KnowledgeId.h"
#include "Timestamp.h"

#include <string>
#include <utility>

namespace xauusd::sovereign {

struct KnowledgeContradiction {
    EntityId    contradiction_id;
    KnowledgeId knowledge_a;
    KnowledgeId knowledge_b;
    std::string shared_scope;
    Timestamp   detected_at;
    bool        resolved;

    KnowledgeContradiction() = default;

    KnowledgeContradiction(
        EntityId contradiction_id,
        KnowledgeId knowledge_a,
        KnowledgeId knowledge_b,
        std::string shared_scope,
        Timestamp detected_at,
        bool resolved)
        : contradiction_id(contradiction_id),
          knowledge_a(knowledge_a),
          knowledge_b(knowledge_b),
          shared_scope(std::move(shared_scope)),
          detected_at(detected_at),
          resolved(resolved) {}
};

} // namespace xauusd::sovereign
