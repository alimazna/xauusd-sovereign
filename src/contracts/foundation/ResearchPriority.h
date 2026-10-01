#pragma once

#include "EntityId.h"
#include "Timestamp.h"

#include <cstdint>
#include <string>
#include <utility>

namespace xauusd::sovereign {

struct ResearchPriority {
    EntityId       entity_id;
    double         score;
    std::string    rationale;
    std::uint64_t  affected_predictions;
    double         evidence_strength;
    Timestamp      computed_at;

    ResearchPriority() = default;

    ResearchPriority(
        EntityId entity_id,
        double score,
        std::string rationale,
        std::uint64_t affected_predictions,
        double evidence_strength,
        Timestamp computed_at)
        : entity_id(entity_id),
          score(score),
          rationale(std::move(rationale)),
          affected_predictions(affected_predictions),
          evidence_strength(evidence_strength),
          computed_at(computed_at) {}
};

} // namespace xauusd::sovereign
