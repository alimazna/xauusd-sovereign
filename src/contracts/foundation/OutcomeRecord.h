#pragma once

#include "EntityId.h"
#include "Timestamp.h"
#include "Outcome.h"

#include <string>
#include <utility>

namespace xauusd::sovereign {

class OutcomeRecord {
public:
    EntityId     record_id;
    Timestamp    recorded_at;
    Outcome      outcome;
    std::string  notes;

    OutcomeRecord() = default;

    OutcomeRecord(
        EntityId record_id,
        Timestamp recorded_at,
        Outcome outcome,
        std::string notes)
        : record_id(record_id),
          recorded_at(recorded_at),
          outcome(std::move(outcome)),
          notes(std::move(notes)) {}
};

} // namespace xauusd::sovereign
