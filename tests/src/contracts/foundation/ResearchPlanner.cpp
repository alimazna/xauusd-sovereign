#include "ResearchPlanner.h"

namespace xauusd::sovereign {

bool ResearchPlanner::start_campaign(const ResearchCampaign& campaign) {
    for (const ResearchCampaign& existing : campaigns_) {
        if (existing.campaign_id == campaign.campaign_id) {
            return false;
        }
    }

    campaigns_.push_back(campaign);
    return true;
}

bool ResearchPlanner::stop_campaign(const EntityId& campaign_id) {
    for (ResearchCampaign& campaign : campaigns_) {
        if (campaign.campaign_id == campaign_id) {
            if (!campaign.active) {
                return false;
            }
            campaign.active = false;
            return true;
        }
    }
    return false;
}

std::vector<ResearchPriority> ResearchPlanner::rank_priorities(
    const std::vector<ResearchPriority>& candidates) const {
    std::vector<ResearchPriority> ranked = candidates;

    // Stable insertion sort keeps equal-score candidates in input order.
    for (std::size_t i = 1; i < ranked.size(); ++i) {
        ResearchPriority current = std::move(ranked[i]);
        std::size_t j = i;
        while (j > 0U && ranked[j - 1U].score < current.score) {
            ranked[j] = std::move(ranked[j - 1U]);
            --j;
        }
        ranked[j] = std::move(current);
    }

    return ranked;
}

std::size_t ResearchPlanner::active_campaign_count() const {
    std::size_t count = 0U;
    for (const ResearchCampaign& campaign : campaigns_) {
        if (campaign.active) {
            ++count;
        }
    }
    return count;
}

void ResearchPlanner::clear() {
    campaigns_.clear();
}

} // namespace xauusd::sovereign
