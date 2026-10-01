#pragma once
#include "EntityId.h"
#include "Timestamp.h"
#include "Version.h"
#include <string>
#include <utility>
namespace xauusd::sovereign {
struct RollbackRequest {
    EntityId     request_id;
    Version      from_version;
    Version      to_version;
    EntityId     incident_id;
    std::string  reason;
    Timestamp    requested_at;
    RollbackRequest() = default;
    RollbackRequest(EntityId request_id, Version from_version,
                    Version to_version, EntityId incident_id,
                    std::string reason, Timestamp requested_at)
        : request_id(request_id), from_version(from_version), to_version(to_version),
          incident_id(incident_id), reason(std::move(reason)), requested_at(requested_at) {}
};
}
