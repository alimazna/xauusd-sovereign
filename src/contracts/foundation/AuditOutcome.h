// src/contracts/foundation/AuditOutcome.h
#pragma once

#include <cstdint>

namespace xauusd::sovereign {

enum class AuditOutcome : std::uint8_t {
    SUCCESS,
    FAILURE,
    PARTIAL,
    REJECTED,
    DEFERRED,
    UNKNOWN
};

} // namespace xauusd::sovereign
