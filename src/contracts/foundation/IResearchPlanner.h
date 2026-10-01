#pragma once

#include "EntityId.h"
#include "ResearchBudget.h"
#include "ResearchCampaign.h"
#include "ResearchPriority.h"

#include <cstddef>
#include <vector>

namespace xauusd::sovereign {

class IResearchPlanner {
public:
    virtual ~IResearchPlanner() = default;

    virtual bool start_campaign(const ResearchCampaign& campaign) = 0;
    virtual bool stop_campaign(const EntityId& campaign_id) = 0;
    virtual std::vector<ResearchPriority> rank_priorities(
        const std::vector<ResearchPriority>& candidates) const = 0;
    virtual std::size_t active_campaign_count() const = 0;
};

} // namespace xauusd::sovereign
