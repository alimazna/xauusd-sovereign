#pragma once

#include "EntityId.h"
#include "Timestamp.h"
#include "Version.h"
#include "Timeframe.h"

#include <string>
#include <utility>

namespace xauusd::sovereign {

class ShadowDecision {
public:
    EntityId       decision_id;
    Timestamp      decision_time;
    Timeframe      trigger_timeframe;
    std::string    strategy_name;
    Version        strategy_version;
    Version        configuration_version;
    std::string    reason;

    ShadowDecision() = default;

    ShadowDecision(
        EntityId decision_id,
        Timestamp decision_time,
        Timeframe trigger_timeframe,
        std::string strategy_name,
        Version strategy_version,
        Version configuration_version,
        std::string reason)
        : decision_id(decision_id),
          decision_time(decision_time),
          trigger_timeframe(trigger_timeframe),
          strategy_name(std::move(strategy_name)),
          strategy_version(strategy_version),
          configuration_version(configuration_version),
          reason(std::move(reason)) {}
};

} // namespace xauusd::sovereign