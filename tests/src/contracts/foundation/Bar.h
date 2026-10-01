#pragma once
#include "Timeframe.h"
#include "Timestamp.h"
#include <cstdint>
namespace xauusd::sovereign {
class Bar {
public:
    Timestamp open_time{}; Timestamp close_time{}; double open{0.0}; double high{0.0}; double low{0.0}; double close{0.0};
    std::uint64_t tick_volume{0}; std::uint64_t real_volume{0}; Timeframe timeframe{Timeframe::M1};
    Bar() = default;
    Bar(Timestamp open_time,Timestamp close_time,double open,double high,double low,double close,std::uint64_t tick_volume,std::uint64_t real_volume,Timeframe timeframe)
        : open_time(open_time),close_time(close_time),open(open),high(high),low(low),close(close),tick_volume(tick_volume),real_volume(real_volume),timeframe(timeframe) {}
};
} // namespace xauusd::sovereign
