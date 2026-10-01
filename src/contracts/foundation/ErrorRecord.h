
#pragma once

#include <cstdint>
#include <string>
#include <utility>

#include "EntityId.h"
#include "Timestamp.h"
#include "ServiceState.h"
#include "ErrorSeverity.h"
#include "RecoveryAction.h"
#include "ErrorCode.h"

namespace xauusd::sovereign {

class ErrorRecord {
public:
    EntityId        error_id;
    std::string     component;
    ErrorSeverity   severity;
    Timestamp       timestamp;
    ServiceState    state;
    ErrorCode       code;
    std::string     message;
    std::string     context;
    RecoveryAction  recovery_action;

    ErrorRecord() = default;

    ErrorRecord(
        EntityId error_id_,
        std::string component_,
        ErrorSeverity severity_,
        Timestamp timestamp_,
        ServiceState state_,
        ErrorCode code_,
        std::string message_,
        std::string context_,
        RecoveryAction recovery_action_)
        : error_id(error_id_),
          component(std::move(component_)),
          severity(severity_),
          timestamp(timestamp_),
          state(state_),
          code(code_),
          message(std::move(message_)),
          context(std::move(context_)),
          recovery_action(recovery_action_) {}
};

} // namespace xauusd::sovereign