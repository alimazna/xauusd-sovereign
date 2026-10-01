#pragma once

#include "EntityId.h"
#include "HashDigest.h"
#include "Timestamp.h"
#include "Version.h"

#include <cstdint>
#include <string>
#include <utility>

namespace xauusd::sovereign {

class PersistenceRecordMetadata {
public:
    EntityId       record_id;
    std::string    record_type;
    Version        schema_version;
    Timestamp      created_at;
    Timestamp      updated_at;
    HashDigest     content_digest;
    std::uint64_t  revision;

    PersistenceRecordMetadata() = default;

    PersistenceRecordMetadata(
        const EntityId& record_id,
        std::string record_type,
        const Version& schema_version,
        const Timestamp& created_at,
        const Timestamp& updated_at,
        const HashDigest& content_digest,
        std::uint64_t revision)
        : record_id(record_id),
          record_type(std::move(record_type)),
          schema_version(schema_version),
          created_at(created_at),
          updated_at(updated_at),
          content_digest(content_digest),
          revision(revision) {}
};

} // namespace xauusd::sovereign
