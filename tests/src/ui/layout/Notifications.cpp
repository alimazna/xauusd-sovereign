#include "Notifications.h"

#include <imgui.h>

namespace xauusd::ui {

void Notifications::render(UiState& state) {
    if (state.status_message.empty()) {
        return;
    }

    const ImGuiIO& io = ImGui::GetIO();
    state.notification_ttl_seconds -= io.DeltaTime;
    if (state.notification_ttl_seconds <= 0.0f) {
        state.status_message.clear();
        return;
    }

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    const ImVec2 position(
        viewport->WorkPos.x + viewport->WorkSize.x - 20.0f,
        viewport->WorkPos.y + 45.0f);
    ImGui::SetNextWindowPos(position, ImGuiCond_Always, ImVec2(1.0f, 0.0f));

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
                             ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                             ImGuiWindowFlags_NoFocusOnAppearing;
    if (ImGui::Begin("##Notification", nullptr, flags)) {
        ImGui::TextUnformatted(state.status_message.c_str());
    }
    ImGui::End();
}

} // namespace xauusd::ui
