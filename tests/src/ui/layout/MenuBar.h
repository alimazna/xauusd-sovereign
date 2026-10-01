#pragma once

#include "../models/UiState.h"

namespace xauusd::ui {

class MenuBar {
public:
    MenuBar() = default;
    void render(UiState& state);
};

} // namespace xauusd::ui
