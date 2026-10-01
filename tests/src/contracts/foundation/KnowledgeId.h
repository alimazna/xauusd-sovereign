#pragma once

#include "EntityId.h"

namespace xauusd::sovereign {

class KnowledgeId {
public:
    KnowledgeId() = default;

    explicit KnowledgeId(const EntityId& entity_id)
        : entity_id_(entity_id) {}

    const EntityId& entity_id() const noexcept {
        return entity_id_;
    }

    friend bool operator==(const KnowledgeId&, const KnowledgeId&) = default;
    friend bool operator!=(const KnowledgeId&, const KnowledgeId&) = default;

private:
    EntityId entity_id_{};
};

} // namespace xauusd::sovereign
