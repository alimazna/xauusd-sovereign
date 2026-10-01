#pragma once

#include "ITimeframeStateStore.h"

#include <cstddef>
#include <map>

namespace xauusd::sovereign {

class TimeframeStateStore final : public ITimeframeStateStore {
public:
    TimeframeStateStore() = default;

    bool get_state(
        Timeframe timeframe,
        TimeframeState& out_state) const override;

    void put_state(const TimeframeState& state) override;

    bool has_state(Timeframe timeframe) const override;

    std::size_t size() const noexcept;

    void clear();

private:
    std::map<Timeframe, TimeframeState> states_;
};

} // namespace xauusd::sovereign
