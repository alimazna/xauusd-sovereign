#pragma once

#include "EntityId.h"
#include "EvaluatorVersion.h"
#include "MetricRegistryVersion.h"

#include <string>
#include <utility>

namespace xauusd::sovereign {

struct EvaluatorIdentity {
    EntityId                evaluator_id;
    EvaluatorVersion        evaluator_version;
    MetricRegistryVersion   metric_registry_version;
    std::string             description;

    EvaluatorIdentity() = default;

    EvaluatorIdentity(EntityId evaluator_id, EvaluatorVersion evaluator_version,
                      MetricRegistryVersion metric_registry_version,
                      std::string description)
        : evaluator_id(evaluator_id),
          evaluator_version(evaluator_version),
          metric_registry_version(metric_registry_version),
          description(std::move(description)) {}
};

} // namespace xauusd::sovereign
