#pragma once

#include "EntityId.h"

namespace xauusd::ui {

struct SelectionState {
    xauusd::sovereign::EntityId selected_candidate{};
    xauusd::sovereign::EntityId selected_prediction{};
    xauusd::sovereign::EntityId selected_hypothesis{};
    xauusd::sovereign::EntityId selected_incident{};
    xauusd::sovereign::EntityId selected_approval{};
    int active_tab{0};
};

} // namespace xauusd::ui
