
#pragma once

#include "EntityId.h"
#include "HashDigest.h"
#include "ProtocolVersion.h"
#include "SchemaVersion.h"
#include "Timestamp.h"

#include <cstdint>
#include <vector>

namespace xauusd::sovereign {

class MessageMetadata {
public:
    ProtocolVersion           protocol_version;
    SchemaVersion             schema_version;
    EntityId                  message_id;
    Timestamp                 timestamp;
    EntityId                  source;
    EntityId                  destination;
    std::vector<std::uint8_t> payload;
    bool                      has_checksum;
    HashDigest                checksum;

    MessageMetadata() = default;

    MessageMetadata(
        const ProtocolVersion& protocol_version,
        const SchemaVersion& schema_version,
        const EntityId& message_id,
        const Timestamp& timestamp,
        const EntityId& source,
        const EntityId& destination,
        std::vector<std::uint8_t> payload,
        bool has_checksum,
        const HashDigest& checksum)
        : protocol_version(protocol_version),
          schema_version(schema_version),
          message_id(message_id),
          timestamp(timestamp),
          source(source),
          destination(destination),
          payload(std::move(payload)),
          has_checksum(has_checksum),
          checksum(checksum) {}
};

} // namespace xauusd::sovereign