#include "ScheduleAdapter.h"

namespace xauusd::ui {

ScheduleAdapter::ScheduleAdapter(
    const xauusd::sovereign::ScheduleManager* schedule,
    const xauusd::sovereign::CheckpointStore* checkpoints) noexcept
    : schedule_(schedule), checkpoints_(checkpoints) {}

ScheduleSnapshot ScheduleAdapter::snapshot() const {
    ScheduleSnapshot result;
    if (schedule_ != nullptr) {
        result.mode = schedule_->current_mode();
        result.events = schedule_->all_events();
    }
    if (checkpoints_ != nullptr) {
        result.checkpoints = checkpoints_->all();
    }
    return result;
}

bool ScheduleAdapter::has_sources() const noexcept {
    return schedule_ != nullptr || checkpoints_ != nullptr;
}

ScheduleSourceStatus ScheduleAdapter::sources() const noexcept {
    return ScheduleSourceStatus{
        schedule_ != nullptr,
        checkpoints_ != nullptr
    };
}

void ScheduleAdapter::bind(
    const xauusd::sovereign::ScheduleManager* schedule,
    const xauusd::sovereign::CheckpointStore* checkpoints) noexcept {
    schedule_ = schedule;
    checkpoints_ = checkpoints;
}

} // namespace xauusd::ui
