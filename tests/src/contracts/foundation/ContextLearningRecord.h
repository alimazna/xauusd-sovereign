#pragma once

#include "ContextKey.h"
#include "ContextValue.h"
#include "EntityId.h"
#include "Timestamp.h"

#include <string>
#include <utility>

namespace xauusd::sovereign {

struct ContextLearningRecord {
    EntityId                record_id;
    ContextKey              key;
    ContextValue            value;
    std::string             observation;
    double                  significance;
    Timestamp               recorded_at;

    ContextLearningRecord() = default;

    ContextLearningRecord(
        EntityId record_id,
        ContextKey key,
        ContextValue value,
        std::string observation,
        double significance,
        Timestamp recorded_at)
        : record_id(record_id),
          key(std::move(key)),
          value(std::move(value)),
          observation(std::move(observation)),
          significance(significance),
          recorded_at(recorded_at) {}
};

} // namespace xauusd::sovereign
