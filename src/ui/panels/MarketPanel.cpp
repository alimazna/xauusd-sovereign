#include "MarketPanel.h"

#include <imgui.h>

namespace xauusd::ui {

void MarketPanel::render(UiState& state) {
    if (!ImGui::Begin("Market", &state.panels.market)) {
        ImGui::End();
        return;
    }

    static int timeframe = 4;
    static const char* labels[] = {"M1", "M5", "M15", "M30", "H1", "H4", "D1", "W1", "MN1"};
    ImGui::Combo("Timeframe", &timeframe, labels, 9);
    ImGui::Separator();
    ImGui::Text("Latest bar/tick: placeholder");
    ImGui::Text("Open / High / Low / Close: unavailable in the Phase 9 source list");
    ImGui::Text("Volume: unavailable in the Phase 9 source list");
    ImGui::TextWrapped("This panel intentionally does not synthesize market values. It is ready to consume a future read-only market adapter.");

    ImGui::End();
}

} // namespace xauusd::ui
