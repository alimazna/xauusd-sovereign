#pragma once

#include "../models/UiState.h"
#include "../adapters/RuntimeAdapter.h"
#include "../adapters/ObservationAdapter.h"
#include "../adapters/GovernanceAdapter.h"

namespace xauusd::ui {

class StatusBar {
public:
    StatusBar() = default;
    void render(
        UiState& state,
        const RuntimeAdapter& runtime,
        const ObservationAdapter& observation,
        const GovernanceAdapter& governance);
};

} // namespace xauusd::ui
