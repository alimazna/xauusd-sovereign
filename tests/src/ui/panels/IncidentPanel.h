#pragma once

#include "../adapters/GovernanceAdapter.h"
#include "../models/UiState.h"

namespace xauusd::ui {

class IncidentPanel {
public:
    IncidentPanel() = default;
    void render(UiState& state, const GovernanceAdapter& governance);
};

} // namespace xauusd::ui
