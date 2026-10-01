#pragma once

#include "EntityId.h"
#include "Timestamp.h"
#include "ValidationMethod.h"

#include <cstdint>
#include <string>
#include <utility>

namespace xauusd::sovereign {

struct ValidationResult {
    EntityId          result_id;
    EntityId          run_id;
    ValidationMethod  method;
    bool              passed;
    double            score;
    double            threshold;
    std::uint64_t     trials;
    std::string       summary;
    Timestamp         recorded_at;

    ValidationResult() = default;

    ValidationResult(EntityId result_id, EntityId run_id,
                     ValidationMethod method, bool passed,
                     double score, double threshold, std::uint64_t trials,
                     std::string summary, Timestamp recorded_at)
        : result_id(result_id),
          run_id(run_id),
          method(method),
          passed(passed),
          score(score),
          threshold(threshold),
          trials(trials),
          summary(std::move(summary)),
          recorded_at(recorded_at) {}
};

} // namespace xauusd::sovereign
