#pragma once

#include "../adapters/ScheduleAdapter.h"
#include "../models/UiState.h"

namespace xauusd::ui {

class SchedulePanel {
public:
    SchedulePanel() = default;
    void render(UiState& state, const ScheduleAdapter& schedule);
};

} // namespace xauusd::ui
