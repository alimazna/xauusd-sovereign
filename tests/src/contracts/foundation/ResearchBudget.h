#pragma once

#include <cstdint>

namespace xauusd::sovereign {

struct ResearchBudget {
    std::uint64_t max_experiments;
    std::uint64_t max_trials;
    std::uint64_t max_candidates;
    std::uint64_t max_holdout_queries;
    std::uint64_t max_compute_units;
};

} // namespace xauusd::sovereign
