#pragma once

#include "EntityId.h"

namespace xauusd::sovereign {

class EventId {
public:
    EventId() = default;

    explicit EventId(const EntityId& entity_id)
        : entity_id_(entity_id) {}

    const EntityId& entity_id() const noexcept {
        return entity_id_;
    }

    friend bool operator==(const EventId&, const EventId&) = default;
    friend bool operator!=(const EventId&, const EventId&) = default;

private:
    EntityId entity_id_{};
};

} // namespace xauusd::sovereign
