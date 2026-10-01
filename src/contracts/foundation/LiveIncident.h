#pragma once

#include "EntityId.h"
#include "IncidentSeverity.h"
#include "Timestamp.h"

#include <string>
#include <utility>

namespace xauusd::sovereign {

struct LiveIncident {
    EntityId         incident_id;
    IncidentSeverity severity;
    std::string      summary;
    std::string      action_taken;
    Timestamp        detected_at;
    bool             resolved;

    LiveIncident() = default;

    LiveIncident(EntityId incident_id, IncidentSeverity severity,
                 std::string summary, std::string action_taken,
                 Timestamp detected_at, bool resolved)
        : incident_id(incident_id), severity(severity),
          summary(std::move(summary)), action_taken(std::move(action_taken)),
          detected_at(detected_at), resolved(resolved) {}
};

} // namespace xauusd::sovereign
