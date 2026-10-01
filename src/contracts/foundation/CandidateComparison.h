#pragma once

#include "EntityId.h"
#include "ComparisonResult.h"
#include "Timestamp.h"

#include <string>
#include <vector>

namespace xauusd::sovereign {

struct CandidateComparison {
    EntityId                     comparison_id;
    std::vector<ComparisonResult> results;
    std::string                  summary;
    Timestamp                    completed_at;
};

} // namespace xauusd::sovereign
