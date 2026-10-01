#include "IncidentPanel.h"

#include <imgui.h>

namespace xauusd::ui {

namespace {
const char* severity_text(xauusd::sovereign::IncidentSeverity value) {
    switch (value) {
        case xauusd::sovereign::IncidentSeverity::INFO: return "INFO";
        case xauusd::sovereign::IncidentSeverity::LOW: return "LOW";
        case xauusd::sovereign::IncidentSeverity::MEDIUM: return "MEDIUM";
        case xauusd::sovereign::IncidentSeverity::HIGH: return "HIGH";
        case xauusd::sovereign::IncidentSeverity::CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}
}

void IncidentPanel::render(UiState& state, const GovernanceAdapter& governance) {
    if (!ImGui::Begin("Incidents", &state.panels.incidents)) {
        ImGui::End();
        return;
    }

    const auto data = governance.snapshot();
    ImGui::Text("Total: %llu  |  Unresolved: %llu",
                static_cast<unsigned long long>(data.incidents.size()),
                static_cast<unsigned long long>(data.unresolved_incidents.size()));

    if (ImGui::BeginTable("incident_table", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp)) {
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("Severity");
        ImGui::TableSetupColumn("Resolved");
        ImGui::TableSetupColumn("Summary");
        ImGui::TableSetupColumn("Root cause");
        ImGui::TableSetupColumn("Action");
        ImGui::TableHeadersRow();

        for (const auto& incident : data.incidents) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            const auto id = format_id(incident.incident_id);
            if (ImGui::Selectable(id.c_str(), state.selection.selected_incident == incident.incident_id)) {
                state.selection.selected_incident = incident.incident_id;
            }
            ImGui::TableNextColumn(); ImGui::TextUnformatted(severity_text(incident.severity));
            ImGui::TableNextColumn(); ImGui::TextUnformatted(incident.resolved ? "YES" : "NO");
            ImGui::TableNextColumn(); ImGui::TextUnformatted(incident.summary.c_str());
            ImGui::TableNextColumn(); ImGui::TextUnformatted(incident.root_cause_hypothesis.c_str());
            ImGui::TableNextColumn();
            if (ImGui::Button(("Resolve##" + id).c_str())) {
                set_notification(state, "Read-only UI: incident resolution requires a governance command bridge.");
            }
        }
        ImGui::EndTable();
    }

    ImGui::End();
}

} // namespace xauusd::ui
