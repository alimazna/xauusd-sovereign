#pragma once

#include "IEvidenceFirewall.h"

#include <cstddef>
#include <vector>

namespace xauusd::sovereign {

class EvidenceFirewall final : public IEvidenceFirewall {
public:
    explicit EvidenceFirewall(std::size_t holdout_budget);
    EvidenceFirewall() : EvidenceFirewall(0) {}

    bool register_layer(const EvidenceLayer& layer) override;
    bool contains_layer(EvidenceZone zone) const override;
    bool get_layer(EvidenceZone zone, EvidenceLayer& out) const override;

    bool request_holdout(const HoldoutQuery& query) override;
    bool consume_holdout(const EntityId& query_id) override;
    std::size_t holdout_query_count() const override;
    std::size_t holdout_budget_remaining() const override;

    bool record_snapshot(const EvidenceSnapshot& snapshot) override;
    std::vector<EvidenceSnapshot> snapshots_for(const EntityId& candidate_id) const override;

    void clear();

private:
    std::size_t holdout_budget_;
    std::size_t holdout_used_{0};
    std::vector<EvidenceLayer> layers_;
    std::vector<HoldoutQuery> queries_;
    std::vector<EvidenceSnapshot> snapshots_;
};

} // namespace xauusd::sovereign
