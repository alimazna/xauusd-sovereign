#pragma once

#include "../adapters/ValidationAdapter.h"
#include "../models/UiState.h"

namespace xauusd::ui {

class ValidationPanel {
public:
    ValidationPanel() = default;
    void render(UiState& state, const ValidationAdapter& validation);
};

} // namespace xauusd::ui
