#pragma once
#include "Timestamp.h"
#include <cstdint>
namespace xauusd::sovereign {
struct ScheduleWindow {
    Timestamp start_time{}; Timestamp end_time{}; std::uint64_t duration_us{0}; std::uint64_t minimum_duration_us{0}; std::uint64_t maximum_duration_us{0}; bool runtime_enabled{false}; bool research_enabled{false};
    ScheduleWindow() = default;
    ScheduleWindow(Timestamp start_time,Timestamp end_time,std::uint64_t duration_us,std::uint64_t minimum_duration_us,std::uint64_t maximum_duration_us,bool runtime_enabled,bool research_enabled)
        : start_time(start_time),end_time(end_time),duration_us(duration_us),minimum_duration_us(minimum_duration_us),maximum_duration_us(maximum_duration_us),runtime_enabled(runtime_enabled),research_enabled(research_enabled) {}
};
}
