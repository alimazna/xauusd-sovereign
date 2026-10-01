#pragma once
#include "Timestamp.h"
#include <cstdint>
namespace xauusd::sovereign {
class RuntimeState {
public:
    Timestamp started_at{}; Timestamp last_activity{};
    std::uint64_t ticks_processed{0}; std::uint64_t bars_processed{0}; std::uint64_t decisions_recorded{0};
    RuntimeState() = default;
    RuntimeState(Timestamp started_at,Timestamp last_activity,std::uint64_t ticks_processed,std::uint64_t bars_processed,std::uint64_t decisions_recorded)
        : started_at(started_at),last_activity(last_activity),ticks_processed(ticks_processed),bars_processed(bars_processed),decisions_recorded(decisions_recorded) {}
};
} // namespace xauusd::sovereign
