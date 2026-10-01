#pragma once

#include "EntityId.h"
#include "ComparisonOutcome.h"
#include "Timestamp.h"

#include <string>
#include <utility>

namespace xauusd::sovereign {

struct ComparisonResult {
    EntityId          control_id;
    EntityId          candidate_id;
    ComparisonOutcome outcome;
    double            primary_metric_control;
    double            primary_metric_candidate;
    double            primary_delta;
    std::string       rationale;
    Timestamp         compared_at;
};

} // namespace xauusd::sovereign
