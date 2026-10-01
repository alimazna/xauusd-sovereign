#pragma once

#include "Timeframe.h"
#include "Bar.h"
#include "Timestamp.h"
#include "DataQualityState.h"
#include "BarFinalizationState.h"

namespace xauusd::sovereign {

class TimeframeState {
public:
    Timeframe               timeframe;
    Bar                     latest_finalized_bar;
    Timestamp               last_update_time;
    DataQualityState        quality;
    BarFinalizationState    finalization;

    TimeframeState() = default;

    TimeframeState(
        Timeframe timeframe,
        Bar latest_finalized_bar,
        Timestamp last_update_time,
        DataQualityState quality,
        BarFinalizationState finalization)
        : timeframe(timeframe),
          latest_finalized_bar(latest_finalized_bar),
          last_update_time(last_update_time),
          quality(quality),
          finalization(finalization) {}
};

} // namespace xauusd::sovereign
