#pragma once

#include "EntityId.h"
#include "Timestamp.h"
#include "Version.h"

#include <string>
#include <utility>

namespace xauusd::sovereign {

struct HoldoutQuery {
    EntityId     query_id;
    EntityId     candidate_id;
    EntityId     dataset_id;
    Version      evaluator_version;
    Timestamp    requested_at;
    std::string  purpose;
    bool         approved;
    bool         consumed;

    HoldoutQuery() = default;

    HoldoutQuery(EntityId query_id, EntityId candidate_id, EntityId dataset_id,
                 Version evaluator_version, Timestamp requested_at,
                 std::string purpose, bool approved, bool consumed)
        : query_id(query_id),
          candidate_id(candidate_id),
          dataset_id(dataset_id),
          evaluator_version(evaluator_version),
          requested_at(requested_at),
          purpose(std::move(purpose)),
          approved(approved),
          consumed(consumed) {}
};

} // namespace xauusd::sovereign
