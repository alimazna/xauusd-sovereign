#pragma once

#include "IResearchPlanner.h"

#include <vector>

namespace xauusd::sovereign {

class ResearchPlanner final : public IResearchPlanner {
public:
    ResearchPlanner() = default;

    bool start_campaign(const ResearchCampaign& campaign) override;
    bool stop_campaign(const EntityId& campaign_id) override;
    std::vector<ResearchPriority> rank_priorities(
        const std::vector<ResearchPriority>& candidates) const override;
    std::size_t active_campaign_count() const override;

    void clear();

private:
    std::vector<ResearchCampaign> campaigns_;
};

} // namespace xauusd::sovereign
