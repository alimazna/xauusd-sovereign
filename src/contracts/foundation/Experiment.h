#pragma once

#include "EntityId.h"
#include "Version.h"
#include "ExperimentFingerprint.h"
#include "ExperimentStatus.h"
#include "Timestamp.h"

#include <cstdint>
#include <string>
#include <utility>

namespace xauusd::sovereign {

class Experiment {
public:
    EntityId              experiment_id;
    EntityId              hypothesis_id;
    Version               parent_version;
    std::string           dataset_version;
    std::string           feature_version;
    std::string           evaluator_version;
    std::string           environment_version;
    ExperimentFingerprint fingerprint;
    ExperimentStatus      status;
    std::uint64_t         trial_count;
    std::uint64_t         trial_budget;
    Timestamp             created_at;
    Timestamp             started_at;

    Experiment() = default;

    Experiment(
        EntityId experiment_id,
        EntityId hypothesis_id,
        Version parent_version,
        std::string dataset_version,
        std::string feature_version,
        std::string evaluator_version,
        std::string environment_version,
        ExperimentFingerprint fingerprint,
        ExperimentStatus status,
        std::uint64_t trial_count,
        std::uint64_t trial_budget,
        Timestamp created_at,
        Timestamp started_at)
        : experiment_id(experiment_id),
          hypothesis_id(hypothesis_id),
          parent_version(parent_version),
          dataset_version(std::move(dataset_version)),
          feature_version(std::move(feature_version)),
          evaluator_version(std::move(evaluator_version)),
          environment_version(std::move(environment_version)),
          fingerprint(std::move(fingerprint)),
          status(status),
          trial_count(trial_count),
          trial_budget(trial_budget),
          created_at(created_at),
          started_at(started_at) {}
};

} // namespace xauusd::sovereign
