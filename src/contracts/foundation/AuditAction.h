// src/contracts/foundation/AuditAction.h
#pragma once

#include <cstdint>

namespace xauusd::sovereign {

enum class AuditAction : std::uint16_t {
    SESSION_STARTED,
    SESSION_ENDED,
    CONFIGURATION_CHANGED,
    STATE_TRANSITION,
    DECISION_RECORDED,
    SIGNAL_PUBLISHED,
    RISK_PROPOSED,
    EXECUTION_PROPOSED,
    HUMAN_DECISION,
    PROMOTION,
    ROLLBACK,
    INCIDENT_RECORDED,
    CHECKPOINT_SAVED,
    CHECKPOINT_RESTORED,
    OTHER
};

} // namespace xauusd::sovereign
