#pragma once

#include "EntityId.h"
#include "Timestamp.h"

#include <string>
#include <utility>

namespace xauusd::sovereign {

struct RCAHypothesis {
    EntityId     hypothesis_id;
    EntityId     failure_pattern_id;
    std::string  description;
    double       confidence;
    bool         confirmed;
    bool         rejected;
    Timestamp    created_at;

    RCAHypothesis() = default;

    RCAHypothesis(
        EntityId hypothesis_id,
        EntityId failure_pattern_id,
        std::string description,
        double confidence,
        bool confirmed,
        bool rejected,
        Timestamp created_at)
        : hypothesis_id(hypothesis_id),
          failure_pattern_id(failure_pattern_id),
          description(std::move(description)),
          confidence(confidence),
          confirmed(confirmed),
          rejected(rejected),
          created_at(created_at) {}
};

} // namespace xauusd::sovereign
