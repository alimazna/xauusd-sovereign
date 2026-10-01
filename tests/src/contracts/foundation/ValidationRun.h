#pragma once

#include "EntityId.h"
#include "ValidationProtocol.h"
#include "Timestamp.h"

#include <string>
#include <utility>

namespace xauusd::sovereign {

struct ValidationRun {
    EntityId            run_id;
    EntityId            candidate_id;
    ValidationProtocol  protocol;
    Timestamp           started_at;
    Timestamp           finished_at;
    bool                completed;
    std::string         notes;

    ValidationRun() = default;

    ValidationRun(EntityId run_id, EntityId candidate_id,
                  ValidationProtocol protocol,
                  Timestamp started_at, Timestamp finished_at,
                  bool completed, std::string notes)
        : run_id(run_id),
          candidate_id(candidate_id),
          protocol(std::move(protocol)),
          started_at(started_at),
          finished_at(finished_at),
          completed(completed),
          notes(std::move(notes)) {}
};

} // namespace xauusd::sovereign
