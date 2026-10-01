#pragma once

#include "../models/UiState.h"

namespace xauusd::ui {

class MarketPanel {
public:
    MarketPanel() = default;
    void render(UiState& state);
};

} // namespace xauusd::ui
