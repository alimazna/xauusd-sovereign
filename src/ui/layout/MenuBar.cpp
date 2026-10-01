#include "MenuBar.h"

#include <imgui.h>

namespace xauusd::ui {

namespace {
void menu_toggle(const char* label, bool& value) {
    if (ImGui::MenuItem(label, nullptr, value)) {
        value = !value;
    }
}
}

void MenuBar::render(UiState& state) {
    if (!ImGui::BeginMainMenuBar()) {
        return;
    }

    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Exit")) {
            state.exit_requested = true;
        }
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("View")) {
        menu_toggle("Dashboard", state.panels.dashboard);
        menu_toggle("Market", state.panels.market);
        menu_toggle("Predictions", state.panels.predictions);
        menu_toggle("Approvals", state.panels.approval);
        menu_toggle("Incidents", state.panels.incidents);
        menu_toggle("Research", state.panels.research);
        menu_toggle("Knowledge", state.panels.knowledge);
        menu_toggle("Candidates", state.panels.candidates);
        menu_toggle("Validation", state.panels.validation);
        menu_toggle("Schedule", state.panels.schedule);
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Help")) {
        if (ImGui::MenuItem("About")) {
            state.panels.about = true;
        }
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
}

} // namespace xauusd::ui
