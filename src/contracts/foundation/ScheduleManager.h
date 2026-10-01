#pragma once
#include "IScheduleManager.h"
#include <vector>
namespace xauusd::sovereign {
class ScheduleManager final : public IScheduleManager {
public:
    ScheduleManager() = default;
    bool configure(const OperatingSchedule& schedule) override;
    OperatingMode current_mode() const override;
    bool transition_to(OperatingMode new_mode, Timestamp at, const std::string& reason) override;
    std::size_t event_count() const override;
    std::vector<ScheduleEvent> all_events() const override;
    void clear();
private:
    OperatingSchedule schedule_;
    OperatingMode mode_{OperatingMode::OFFLINE};
    std::vector<ScheduleEvent> events_;
};
}
