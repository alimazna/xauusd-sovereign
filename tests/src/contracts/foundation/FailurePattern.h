#pragma once

#include "EntityId.h"
#include "Timestamp.h"
#include "FailureType.h"

#include <cstdint>
#include <string>
#include <utility>

namespace xauusd::sovereign {

class FailurePattern {
public:
    EntityId       pattern_id;
    FailureType    failure_type;
    Timestamp      first_seen;
    Timestamp      last_seen;
    std::uint64_t  occurrence_count;
    std::string    context;
    std::string    description;

    FailurePattern() = default;

    FailurePattern(
        EntityId pattern_id,
        FailureType failure_type,
        Timestamp first_seen,
        Timestamp last_seen,
        std::uint64_t occurrence_count,
        std::string context,
        std::string description)
        : pattern_id(pattern_id),
          failure_type(failure_type),
          first_seen(first_seen),
          last_seen(last_seen),
          occurrence_count(occurrence_count),
          context(std::move(context)),
          description(std::move(description)) {}
};

} // namespace xauusd::sovereign
