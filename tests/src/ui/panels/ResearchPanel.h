#pragma once

#include "../adapters/ResearchAdapter.h"
#include "../models/UiState.h"

namespace xauusd::ui {

class ResearchPanel {
public:
    ResearchPanel() = default;
    void render(UiState& state, const ResearchAdapter& research);
};

} // namespace xauusd::ui
