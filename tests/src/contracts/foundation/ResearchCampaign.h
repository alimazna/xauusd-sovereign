#pragma once

#include "EntityId.h"
#include "ResearchBudget.h"
#include "Timestamp.h"

#include <string>
#include <utility>

namespace xauusd::sovereign {

struct ResearchCampaign {
    EntityId       campaign_id;
    EntityId       hypothesis_id;
    std::string    research_goal;
    ResearchBudget budget;
    Timestamp      started_at;
    Timestamp      deadline_at;
    bool           active;

    ResearchCampaign() = default;

    ResearchCampaign(
        EntityId campaign_id,
        EntityId hypothesis_id,
        std::string research_goal,
        ResearchBudget budget,
        Timestamp started_at,
        Timestamp deadline_at,
        bool active)
        : campaign_id(campaign_id),
          hypothesis_id(hypothesis_id),
          research_goal(std::move(research_goal)),
          budget(budget),
          started_at(started_at),
          deadline_at(deadline_at),
          active(active) {}
};

} // namespace xauusd::sovereign
