#pragma once

#include "EntityId.h"
#include "ExperimentStatus.h"
#include "Timestamp.h"

#include <string>
#include <utility>

namespace xauusd::sovereign {

class ExperimentResult {
public:
    EntityId         result_id;
    EntityId         experiment_id;
    ExperimentStatus status;
    std::string      summary;
    double           primary_metric;
    double           secondary_metric;
    Timestamp        recorded_at;

    ExperimentResult() = default;

    ExperimentResult(
        EntityId result_id,
        EntityId experiment_id,
        ExperimentStatus status,
        std::string summary,
        double primary_metric,
        double secondary_metric,
        Timestamp recorded_at)
        : result_id(result_id),
          experiment_id(experiment_id),
          status(status),
          summary(std::move(summary)),
          primary_metric(primary_metric),
          secondary_metric(secondary_metric),
          recorded_at(recorded_at) {}
};

} // namespace xauusd::sovereign
