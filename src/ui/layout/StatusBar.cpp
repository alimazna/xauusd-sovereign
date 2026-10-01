#include "StatusBar.h"

#include <imgui.h>

namespace xauusd::ui {

void StatusBar::render(
    UiState& state,
    const RuntimeAdapter& runtime,
    const ObservationAdapter& observation,
    const GovernanceAdapter& governance) {
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    const float height = ImGui::GetFrameHeightWithSpacing();
    ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x, viewport->WorkPos.y + viewport->WorkSize.y - height));
    ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x, height));

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
                             ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing;
    if (!ImGui::Begin("##StatusBar", nullptr, flags)) {
        ImGui::End();
        return;
    }

    const auto runtime_state = runtime.snapshot();
    const auto observation_state = observation.snapshot();
    const auto governance_state = governance.snapshot();

    ImGui::Text("Runtime: %s", runtime.has_engine() ? "BOUND" : "UNBOUND");
    ImGui::SameLine();
    ImGui::Text("Bars %llu", static_cast<unsigned long long>(runtime_state.bars_processed));
    ImGui::SameLine();
    ImGui::Text("Predictions %llu", static_cast<unsigned long long>(observation_state.predictions.size()));
    ImGui::SameLine();
    ImGui::Text("Incidents %llu", static_cast<unsigned long long>(governance_state.unresolved_incidents.size()));
    if (!state.status_message.empty()) {
        ImGui::SameLine();
        ImGui::Text("| %s", state.status_message.c_str());
    }

    ImGui::End();
}

} // namespace xauusd::ui
