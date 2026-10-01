#pragma once

#include "ScheduleManager.h"
#include "CheckpointStore.h"

#include <vector>

namespace xauusd::ui {

struct ScheduleSourceStatus {
    bool schedule_bound{false};
    bool checkpoints_bound{false};
};

struct ScheduleSnapshot {
    xauusd::sovereign::OperatingMode mode{xauusd::sovereign::OperatingMode::OFFLINE};
    std::vector<xauusd::sovereign::ScheduleEvent> events;
    std::vector<xauusd::sovereign::Checkpoint> checkpoints;
};

class ScheduleAdapter {
public:
    ScheduleAdapter(
        const xauusd::sovereign::ScheduleManager* schedule,
        const xauusd::sovereign::CheckpointStore* checkpoints) noexcept;

    ScheduleSnapshot snapshot() const;
    bool has_sources() const noexcept;
    ScheduleSourceStatus sources() const noexcept;

    void bind(
        const xauusd::sovereign::ScheduleManager* schedule,
        const xauusd::sovereign::CheckpointStore* checkpoints) noexcept;

private:
    const xauusd::sovereign::ScheduleManager* schedule_{nullptr};
    const xauusd::sovereign::CheckpointStore* checkpoints_{nullptr};
};

} // namespace xauusd::ui
