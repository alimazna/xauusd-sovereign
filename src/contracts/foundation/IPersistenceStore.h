#pragma once

#include "PersistenceStatus.h"
#include "PersistenceRecordMetadata.h"
#include "EntityId.h"

#include <cstdint>
#include <span>
#include <vector>

namespace xauusd::sovereign {

class IPersistenceStore {
public:
    virtual ~IPersistenceStore() = default;

    virtual PersistenceStatus put(
        const PersistenceRecordMetadata& metadata,
        std::span<const std::uint8_t> payload) = 0;

    virtual PersistenceStatus get(
        const EntityId& record_id,
        std::vector<std::uint8_t>& out_payload,
        PersistenceRecordMetadata& out_metadata) const = 0;

    virtual PersistenceStatus remove(
        const EntityId& record_id) = 0;

    virtual PersistenceStatus contains(
        const EntityId& record_id,
        bool& out_exists) const = 0;
};

} // namespace xauusd::sovereign
