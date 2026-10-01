#pragma once

#include "EntityId.h"
#include "Timestamp.h"
#include "Version.h"
#include "Timeframe.h"
#include "ShadowDecision.h"
#include "PredictionDirection.h"

#include <string>

namespace xauusd::sovereign {

class Prediction {
public:
    EntityId             prediction_id;
    EntityId             source_decision_id;
    Timestamp            prediction_time;
    Timeframe            trigger_timeframe;
    PredictionDirection  direction;
    std::string          strategy_name;
    Version              strategy_version;
    Version              configuration_version;
    std::string          rationale;

    Prediction() = default;

    Prediction(
        EntityId prediction_id,
        EntityId source_decision_id,
        Timestamp prediction_time,
        Timeframe trigger_timeframe,
        PredictionDirection direction,
        std::string strategy_name,
        Version strategy_version,
        Version configuration_version,
        std::string rationale)
        : prediction_id(prediction_id),
          source_decision_id(source_decision_id),
          prediction_time(prediction_time),
          trigger_timeframe(trigger_timeframe),
          direction(direction),
          strategy_name(std::move(strategy_name)),
          strategy_version(strategy_version),
          configuration_version(configuration_version),
          rationale(std::move(rationale)) {}
};

} // namespace xauusd::sovereign
