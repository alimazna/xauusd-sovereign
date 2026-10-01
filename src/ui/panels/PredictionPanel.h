#pragma once

#include "../adapters/ObservationAdapter.h"
#include "../models/UiState.h"

namespace xauusd::ui {

class PredictionPanel {
public:
    PredictionPanel() = default;
    void render(UiState& state, const ObservationAdapter& observation);
};

} // namespace xauusd::ui
