#pragma once

#include "EntityId.h"
#include "Timestamp.h"
#include "ShadowDecision.h"

#include <string>

namespace xauusd::sovereign {

class ShadowLedgerEntry {
public:
    EntityId        entry_id;
    Timestamp       recorded_at;
    ShadowDecision  decision;
    std::string     notes;

    ShadowLedgerEntry() = default;

    ShadowLedgerEntry(
        EntityId entry_id,
        Timestamp recorded_at,
        ShadowDecision decision,
        std::string notes)
        : entry_id(entry_id),
          recorded_at(recorded_at),
          decision(std::move(decision)),
          notes(std::move(notes)) {}
};

} // namespace xauusd::sovereign
