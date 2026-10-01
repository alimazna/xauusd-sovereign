#pragma once

#include "EntityId.h"
#include "LiveConfig.h"
#include "LiveDecision.h"
#include "LiveModeStatus.h"
#include "LiveSession.h"

#include <cstddef>
#include <vector>

namespace xauusd::sovereign {

class ILiveController {
public:
    virtual ~ILiveController() = default;

    virtual bool enable(const LiveConfig& config) = 0;
    virtual bool disable() = 0;
    virtual bool is_enabled() const = 0;
    virtual LiveModeStatus current_status() const = 0;

    virtual bool record_decision(const LiveDecision& decision) = 0;
    virtual std::vector<LiveDecision> all_decisions() const = 0;

    virtual LiveSession current_session() const = 0;
    virtual std::size_t decision_count() const = 0;
};

} // namespace xauusd::sovereign
