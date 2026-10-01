#pragma once
#include "ScheduleWindow.h"
#include <cstdint>
#include <string>
#include <utility>
#include <vector>
namespace xauusd::sovereign {
struct OperatingSchedule {
    std::vector<ScheduleWindow> windows{}; std::string timezone{"UTC"}; std::uint64_t daily_minimum_us{0}; std::uint64_t daily_maximum_us{0};
    OperatingSchedule() = default;
    OperatingSchedule(std::vector<ScheduleWindow> windows,std::string timezone,std::uint64_t daily_minimum_us,std::uint64_t daily_maximum_us)
        : windows(std::move(windows)),timezone(std::move(timezone)),daily_minimum_us(daily_minimum_us),daily_maximum_us(daily_maximum_us) {}
};
}
