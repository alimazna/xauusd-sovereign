#include "DashboardPanel.h"

#include <imgui.h>

namespace xauusd::ui {

namespace {
const char* mode_text(bool bound) {
    return bound ? "BOUND" : "UNBOUND";
}
}

void DashboardPanel::render(
    UiState& state,
    const RuntimeAdapter& runtime,
    const ObservationAdapter& observation,
    const GovernanceAdapter& governance) {
    if (!ImGui::Begin("Dashboard", &state.panels.dashboard)) {
        ImGui::End();
        return;
    }

    const auto runtime_state = runtime.snapshot();
    const auto observation_state = observation.snapshot();
    const auto governance_state = governance.snapshot();
    const auto observation_sources = observation.sources();
    const auto governance_sources = governance.sources();

    ImGui::Text("System mode: %s", mode_text(runtime.has_engine()));
    ImGui::Text("Health: UNKNOWN (no health aggregate exposed to the Phase 9 adapters)");
    ImGui::Separator();

    if (ImGui::BeginTable("dashboard_metrics", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("Domain");
        ImGui::TableSetupColumn("Count");
        ImGui::TableSetupColumn("Source status");
        ImGui::TableHeadersRow();

        const auto row = [](const char* domain, std::uint64_t count, const char* source) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::TextUnformatted(domain);
            ImGui::TableNextColumn(); ImGui::Text("%llu", static_cast<unsigned long long>(count));
            ImGui::TableNextColumn(); ImGui::TextUnformatted(source);
        };

        row("Bars processed", runtime_state.bars_processed, runtime.has_engine() ? "bound" : "placeholder");
        row("Ticks processed", runtime_state.ticks_processed, runtime.has_engine() ? "bound" : "placeholder");
        row("Decisions recorded", runtime_state.decisions_recorded, runtime.has_engine() ? "bound" : "placeholder");
        row("Predictions", static_cast<std::uint64_t>(observation_state.predictions.size()), observation_sources.predictions_bound ? "bound" : "placeholder");
        row("Outcomes", static_cast<std::uint64_t>(observation_state.outcomes.size()), observation_sources.outcomes_bound ? "bound" : "placeholder");
        row("Failure patterns", static_cast<std::uint64_t>(observation_state.failure_patterns.size()), observation_sources.failures_bound ? "bound" : "placeholder");
        row("Pending approvals", static_cast<std::uint64_t>(governance_state.pending_approvals), governance_sources.approvals_bound ? "bound" : "placeholder");
        row("Unresolved incidents", static_cast<std::uint64_t>(governance_state.unresolved_incidents.size()), governance_sources.incidents_bound ? "bound" : "placeholder");
        ImGui::EndTable();
    }

    if (!runtime.has_engine() && !observation.has_sources() && !governance.has_sources()) {
        ImGui::TextUnformatted("No foundation sources are attached. The prototype remains usable with explicit placeholder state.");
    }

    ImGui::End();
}

} // namespace xauusd::ui
