#pragma once

#include "EntityId.h"
#include "SandboxStatus.h"
#include "SandboxConfig.h"
#include "Timestamp.h"

#include <string>
#include <utility>

namespace xauusd::sovereign {

struct SandboxRun {
    EntityId      run_id;
    EntityId      experiment_id;
    SandboxStatus status;
    SandboxConfig config;
    Timestamp     started_at;
    Timestamp     finished_at;
    std::string   outcome_note;

    SandboxRun() = default;

    SandboxRun(
        EntityId run_id,
        EntityId experiment_id,
        SandboxStatus status,
        SandboxConfig config,
        Timestamp started_at,
        Timestamp finished_at,
        std::string outcome_note)
        : run_id(run_id),
          experiment_id(experiment_id),
          status(status),
          config(std::move(config)),
          started_at(started_at),
          finished_at(finished_at),
          outcome_note(std::move(outcome_note)) {}
};

} // namespace xauusd::sovereign
