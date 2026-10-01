#pragma once

#include "ICandidateComparator.h"

namespace xauusd::sovereign {

class CandidateComparator final : public ICandidateComparator {
public:
    CandidateComparator() = default;

    ComparisonResult compare(
        const EntityId& control_id,
        const EntityId& candidate_id,
        double control_metric,
        double candidate_metric) const override;

    CandidateComparison compare_all(
        const EntityId& control_id,
        const std::vector<EntityId>& candidate_ids,
        const std::vector<double>& candidate_metrics,
        double control_metric) const override;
};

} // namespace xauusd::sovereign
