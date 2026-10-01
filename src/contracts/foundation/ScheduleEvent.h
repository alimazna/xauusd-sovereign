#pragma once
#include "OperatingMode.h"
#include "Timestamp.h"
#include <string>
#include <utility>
namespace xauusd::sovereign {
struct ScheduleEvent {
    Timestamp      event_time;
    OperatingMode  from_mode;
    OperatingMode  to_mode;
    std::string    reason;
    ScheduleEvent() = default;
    ScheduleEvent(Timestamp event_time, OperatingMode from_mode,
                  OperatingMode to_mode, std::string reason)
        : event_time(event_time), from_mode(from_mode), to_mode(to_mode), reason(std::move(reason)) {}
};
}
