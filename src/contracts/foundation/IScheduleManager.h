#pragma once
#include "OperatingMode.h"
#include "OperatingSchedule.h"
#include "ScheduleEvent.h"
#include "Timestamp.h"
#include <cstddef>
#include <string>
#include <vector>
namespace xauusd::sovereign {
class IScheduleManager {
public:
    virtual ~IScheduleManager() = default;
    virtual bool configure(const OperatingSchedule& schedule) = 0;
    virtual OperatingMode current_mode() const = 0;
    virtual bool transition_to(OperatingMode new_mode, Timestamp at,
                               const std::string& reason) = 0;
    virtual std::size_t event_count() const = 0;
    virtual std::vector<ScheduleEvent> all_events() const = 0;
};
}
