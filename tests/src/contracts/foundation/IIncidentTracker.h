#pragma once
#include "EntityId.h"
#include "Incident.h"
#include <cstddef>
#include <vector>
namespace xauusd::sovereign {
class IIncidentTracker {
public:
    virtual ~IIncidentTracker() = default;
    virtual bool record(const Incident& incident) = 0;
    virtual bool contains(const EntityId& incident_id) const = 0;
    virtual bool get(const EntityId& incident_id, Incident& out) const = 0;
    virtual bool resolve(const EntityId& incident_id) = 0;
    virtual std::vector<Incident> all() const = 0;
    virtual std::vector<Incident> unresolved() const = 0;
    virtual std::size_t size() const = 0;
};
}
