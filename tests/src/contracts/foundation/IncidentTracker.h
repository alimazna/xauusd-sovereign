#pragma once
#include "IIncidentTracker.h"
#include <array>
#include <cstdint>
#include <map>
#include <vector>
namespace xauusd::sovereign {
class IncidentTracker final : public IIncidentTracker {
public:
    IncidentTracker() = default;
    bool record(const Incident& incident) override;
    bool contains(const EntityId& incident_id) const override;
    bool get(const EntityId& incident_id, Incident& out) const override;
    bool resolve(const EntityId& incident_id) override;
    std::vector<Incident> all() const override;
    std::vector<Incident> unresolved() const override;
    std::size_t size() const override;
    void clear();
private:
    std::vector<Incident> incidents_;
    std::map<std::array<std::uint8_t, 16>, std::size_t> index_;
};
}
