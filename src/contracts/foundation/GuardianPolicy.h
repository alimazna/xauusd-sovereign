#pragma once

#include "EntityId.h"
#include "Version.h"
#include "GuardianStatus.h"

#include <cstdint>

namespace xauusd::sovereign {

class GuardianPolicy {
public:
    EntityId       policy_id;
    Version        policy_version;
    GuardianStatus active_status = GuardianStatus::NORMAL;
    bool           allow_research_when_degraded = true;
    bool           allow_shadow_when_degraded = true;
    bool           allow_execution_when_degraded = false;
    bool           requires_human_intervention = true;
    std::uint64_t  max_observe_duration_ms = 0;

    GuardianPolicy() = default;

    GuardianPolicy(
        const EntityId& policy_id,
        const Version& policy_version,
        GuardianStatus active_status,
        bool allow_research_when_degraded,
        bool allow_shadow_when_degraded,
        bool allow_execution_when_degraded,
        bool requires_human_intervention,
        std::uint64_t max_observe_duration_ms)
        : policy_id(policy_id),
          policy_version(policy_version),
          active_status(active_status),
          allow_research_when_degraded(allow_research_when_degraded),
          allow_shadow_when_degraded(allow_shadow_when_degraded),
          allow_execution_when_degraded(allow_execution_when_degraded),
          requires_human_intervention(requires_human_intervention),
          max_observe_duration_ms(max_observe_duration_ms) {}
};

} // namespace xauusd::sovereign