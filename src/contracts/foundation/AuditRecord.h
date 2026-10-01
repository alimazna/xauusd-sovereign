// src/contracts/foundation/AuditRecord.h
#pragma once

#include "AuditAction.h"
#include "AuditOutcome.h"
#include "EntityId.h"
#include "Timestamp.h"

#include <string>
#include <utility>

namespace xauusd::sovereign {

class AuditRecord {
public:
    EntityId audit_id;
    AuditAction action;
    AuditOutcome outcome;
    Timestamp timestamp;
    EntityId actor;
    std::string subject;
    std::string details;

    AuditRecord() = default;

    AuditRecord(
        EntityId audit_id,
        AuditAction action,
        AuditOutcome outcome,
        Timestamp timestamp,
        EntityId actor,
        std::string subject,
        std::string details)
        : audit_id(audit_id),
          action(action),
          outcome(outcome),
          timestamp(timestamp),
          actor(actor),
          subject(std::move(subject)),
          details(std::move(details)) {}
};

} // namespace xauusd::sovereign
