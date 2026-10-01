#pragma once

#include "Timeframe.h"
#include "TimeframeState.h"

namespace xauusd::sovereign {

class ITimeframeStateStore {
public:
    virtual ~ITimeframeStateStore() = default;

    virtual bool get_state(
        Timeframe timeframe,
        TimeframeState& out_state) const = 0;

    virtual void put_state(const TimeframeState& state) = 0;

    virtual bool has_state(Timeframe timeframe) const = 0;
};

} // namespace xauusd::sovereign
