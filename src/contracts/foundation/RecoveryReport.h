#pragma once
#include "EntityId.h"
#include "RecoveryPhase.h"
#include "Timestamp.h"
#include <string>
#include <utility>
namespace xauusd::sovereign {
struct RecoveryReport {
    EntityId      report_id;
    EntityId      plan_id;
    RecoveryPhase final_phase;
    bool          success;
    std::string   notes;
    Timestamp     completed_at;
    RecoveryReport() = default;
    RecoveryReport(EntityId report_id, EntityId plan_id,
                   RecoveryPhase final_phase, bool success,
                   std::string notes, Timestamp completed_at)
        : report_id(report_id), plan_id(plan_id), final_phase(final_phase), success(success),
          notes(std::move(notes)), completed_at(completed_at) {}
};
}
