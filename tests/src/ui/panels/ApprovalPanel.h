#pragma once

#include "../adapters/GovernanceAdapter.h"
#include "../models/UiState.h"

namespace xauusd::ui {

class ApprovalPanel {
public:
    ApprovalPanel() = default;
    void render(UiState& state, const GovernanceAdapter& governance);
};

} // namespace xauusd::ui
