#pragma once
#include "EntityId.h"
#include "IncidentSeverity.h"
#include "Timestamp.h"
#include "Version.h"
#include <string>
#include <utility>
namespace xauusd::sovereign {
struct Incident {
    EntityId           incident_id;
    IncidentSeverity   severity;
    EntityId           affected_candidate_id;
    Version            affected_version;
    std::string        summary;
    std::string        root_cause_hypothesis;
    std::string        prevention_actions;
    Timestamp          detected_at;
    Timestamp          recorded_at;
    bool               resolved;
    Incident() = default;
    Incident(EntityId incident_id, IncidentSeverity severity,
             EntityId affected_candidate_id, Version affected_version,
             std::string summary, std::string root_cause_hypothesis,
             std::string prevention_actions, Timestamp detected_at,
             Timestamp recorded_at, bool resolved)
        : incident_id(incident_id), severity(severity),
          affected_candidate_id(affected_candidate_id), affected_version(affected_version),
          summary(std::move(summary)), root_cause_hypothesis(std::move(root_cause_hypothesis)),
          prevention_actions(std::move(prevention_actions)), detected_at(detected_at),
          recorded_at(recorded_at), resolved(resolved) {}
};
}
