#pragma once

#include "FailurePattern.h"
#include "RCAHypothesis.h"
#include "Timestamp.h"

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace xauusd::sovereign {

struct FailureMemoryEntry {
    EntityId                    entry_id;
    FailurePattern              pattern;
    std::vector<RCAHypothesis>  hypotheses;
    std::vector<std::string>    prevention_actions;
    std::uint64_t               recurrence_count;
    Timestamp                   first_recorded_at;
    Timestamp                   last_recorded_at;

    FailureMemoryEntry() = default;

    FailureMemoryEntry(
        EntityId entry_id,
        FailurePattern pattern,
        std::vector<RCAHypothesis> hypotheses,
        std::vector<std::string> prevention_actions,
        std::uint64_t recurrence_count,
        Timestamp first_recorded_at,
        Timestamp last_recorded_at)
        : entry_id(entry_id),
          pattern(pattern),
          hypotheses(std::move(hypotheses)),
          prevention_actions(std::move(prevention_actions)),
          recurrence_count(recurrence_count),
          first_recorded_at(first_recorded_at),
          last_recorded_at(last_recorded_at) {}
};

} // namespace xauusd::sovereign
