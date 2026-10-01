#pragma once

#include "EntityId.h"
#include "EvidenceLayer.h"
#include "EvidenceSnapshot.h"
#include "HoldoutQuery.h"

#include <cstddef>
#include <vector>

namespace xauusd::sovereign {

class IEvidenceFirewall {
public:
    virtual ~IEvidenceFirewall() = default;

    virtual bool register_layer(const EvidenceLayer& layer) = 0;
    virtual bool contains_layer(EvidenceZone zone) const = 0;
    virtual bool get_layer(EvidenceZone zone, EvidenceLayer& out) const = 0;

    virtual bool request_holdout(const HoldoutQuery& query) = 0;
    virtual bool consume_holdout(const EntityId& query_id) = 0;
    virtual std::size_t holdout_query_count() const = 0;
    virtual std::size_t holdout_budget_remaining() const = 0;

    virtual bool record_snapshot(const EvidenceSnapshot& snapshot) = 0;
    virtual std::vector<EvidenceSnapshot> snapshots_for(const EntityId& candidate_id) const = 0;
};

} // namespace xauusd::sovereign
