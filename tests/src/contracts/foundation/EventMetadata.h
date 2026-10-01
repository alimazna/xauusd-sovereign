
#pragma once

#include "EntityId.h"
#include "EventId.h"
#include "EventType.h"
#include "SchemaVersion.h"
#include "Timestamp.h"

#include <cstdint>
#include <string>

namespace xauusd::sovereign {

class EventMetadata {
public:
    EventId       event_id;
    EventType     event_type;
    std::string   symbol;
    EntityId      source;
    EntityId      source_instance;
    Timestamp     event_time;
    Timestamp     receive_time;
    std::uint64_t sequence_id;
    SchemaVersion schema_version;

    EventMetadata() = default;

    EventMetadata(
        const EventId& event_id,
        EventType event_type,
        std::string symbol,
        const EntityId& source,
        const EntityId& source_instance,
        const Timestamp& event_time,
        const Timestamp& receive_time,
        std::uint64_t sequence_id,
        const SchemaVersion& schema_version)
        : event_id(event_id),
          event_type(event_type),
          symbol(std::move(symbol)),
          source(source),
          source_instance(source_instance),
          event_time(event_time),
          receive_time(receive_time),
          sequence_id(sequence_id),
          schema_version(schema_version) {}
};

} // namespace xauusd::sovereign