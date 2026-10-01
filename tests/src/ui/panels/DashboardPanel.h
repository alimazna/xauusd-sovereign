#pragma once

#include "../adapters/RuntimeAdapter.h"
#include "../adapters/ObservationAdapter.h"
#include "../adapters/GovernanceAdapter.h"
#include "../models/UiState.h"

namespace xauusd::ui {

class DashboardPanel {
public:
    DashboardPanel() = default;
    void render(
        UiState& state,
        const RuntimeAdapter& runtime,
        const ObservationAdapter& observation,
        const GovernanceAdapter& governance);
};

} // namespace xauusd::ui
