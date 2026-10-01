#pragma once

#include "EntityId.h"
#include "LiveConfig.h"
#include "LiveModeStatus.h"
#include "Timestamp.h"

#include <cstdint>
#include <utility>

namespace xauusd::sovereign {

struct LiveSession {
    EntityId       session_id;
    LiveModeStatus status;
    LiveConfig     config;
    Timestamp      started_at;
    Timestamp      ended_at;
    std::uint64_t  trades_executed;
    std::uint64_t  trades_rejected;

    LiveSession() = default;

    LiveSession(EntityId session_id, LiveModeStatus status,
                LiveConfig config, Timestamp started_at,
                Timestamp ended_at, std::uint64_t trades_executed,
                std::uint64_t trades_rejected)
        : session_id(session_id), status(status),
          config(std::move(config)), started_at(started_at),
          ended_at(ended_at), trades_executed(trades_executed),
          trades_rejected(trades_rejected) {}
};

} // namespace xauusd::sovereign
