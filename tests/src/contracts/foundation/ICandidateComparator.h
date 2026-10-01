#pragma once

#include "CandidateComparison.h"
#include "ComparisonResult.h"
#include "EntityId.h"

#include <vector>

namespace xauusd::sovereign {

class ICandidateComparator {
public:
    virtual ~ICandidateComparator() = default;

    virtual ComparisonResult compare(
        const EntityId& control_id,
        const EntityId& candidate_id,
        double control_metric,
        double candidate_metric) const = 0;

    virtual CandidateComparison compare_all(
        const EntityId& control_id,
        const std::vector<EntityId>& candidate_ids,
        const std::vector<double>& candidate_metrics,
        double control_metric) const = 0;
};

} // namespace xauusd::sovereign
