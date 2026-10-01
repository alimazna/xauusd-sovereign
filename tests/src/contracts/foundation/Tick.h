#pragma once
#include "Timestamp.h"
#include <cstdint>
namespace xauusd::sovereign {
class Tick {
public:
    Timestamp event_time{}; Timestamp receive_time{}; double bid{0.0}; double ask{0.0}; double last{0.0}; double volume{0.0}; std::uint32_t flags{0};
    Tick() = default;
    Tick(Timestamp event_time,Timestamp receive_time,double bid,double ask,double last,double volume,std::uint32_t flags)
        : event_time(event_time),receive_time(receive_time),bid(bid),ask(ask),last(last),volume(volume),flags(flags) {}
};
} // namespace xauusd::sovereign
