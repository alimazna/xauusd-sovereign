#pragma once

#include "../adapters/LearningAdapter.h"
#include "../models/UiState.h"

namespace xauusd::ui {

class KnowledgePanel {
public:
    KnowledgePanel() = default;
    void render(UiState& state, const LearningAdapter& learning);
};

} // namespace xauusd::ui
