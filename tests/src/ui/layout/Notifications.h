#pragma once

#include "../models/UiState.h"

namespace xauusd::ui {

class Notifications {
public:
    Notifications() = default;
    void render(UiState& state);
};

} // namespace xauusd::ui
