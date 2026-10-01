#pragma once

#include "../adapters/EvolutionAdapter.h"
#include "../models/UiState.h"

namespace xauusd::ui {

class CandidatePanel {
public:
    CandidatePanel() = default;
    void render(UiState& state, const EvolutionAdapter& evolution);
};

} // namespace xauusd::ui
