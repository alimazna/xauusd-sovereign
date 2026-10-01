#include "ResearchPanel.h"

#include <imgui.h>

namespace xauusd::ui {

namespace {
const char* hypothesis_status_text(xauusd::sovereign::HypothesisStatus value) {
    switch (value) {
        case xauusd::sovereign::HypothesisStatus::DRAFTED: return "DRAFTED";
        case xauusd::sovereign::HypothesisStatus::SUBMITTED: return "SUBMITTED";
        case xauusd::sovereign::HypothesisStatus::UNDER_EXPERIMENT: return "UNDER_EXPERIMENT";
        case xauusd::sovereign::HypothesisStatus::SUPPORTED: return "SUPPORTED";
        case xauusd::sovereign::HypothesisStatus::REFUTED: return "REFUTED";
        case xauusd::sovereign::HypothesisStatus::INCONCLUSIVE: return "INCONCLUSIVE";
        case xauusd::sovereign::HypothesisStatus::WITHDRAWN: return "WITHDRAWN";
        default: return "UNKNOWN";
    }
}
const char* experiment_status_text(xauusd::sovereign::ExperimentStatus value) {
    switch (value) {
        case xauusd::sovereign::ExperimentStatus::PLANNED: return "PLANNED";
        case xauusd::sovereign::ExperimentStatus::RUNNING: return "RUNNING";
        case xauusd::sovereign::ExperimentStatus::COMPLETED: return "COMPLETED";
        case xauusd::sovereign::ExperimentStatus::FAILED: return "FAILED";
        case xauusd::sovereign::ExperimentStatus::CANCELLED: return "CANCELLED";
        case xauusd::sovereign::ExperimentStatus::INCONCLUSIVE: return "INCONCLUSIVE";
        case xauusd::sovereign::ExperimentStatus::CONTAMINATED: return "CONTAMINATED";
        default: return "UNKNOWN";
    }
}
}

void ResearchPanel::render(UiState& state, const ResearchAdapter& research) {
    if (!ImGui::Begin("Research", &state.panels.research)) {
        ImGui::End();
        return;
    }

    const auto data = research.snapshot();
    ImGui::Text("Hypotheses: %llu  |  Experiments: %llu",
                static_cast<unsigned long long>(data.hypotheses.size()),
                static_cast<unsigned long long>(data.experiments.size()));

    if (ImGui::BeginTable("hypothesis_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("Status");
        ImGui::TableSetupColumn("Confidence");
        ImGui::TableSetupColumn("Claim");
        ImGui::TableHeadersRow();
        for (const auto& h : data.hypotheses) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            const auto id = format_id(h.hypothesis_id);
            if (ImGui::Selectable(id.c_str(), state.selection.selected_hypothesis == h.hypothesis_id)) {
                state.selection.selected_hypothesis = h.hypothesis_id;
            }
            ImGui::TableNextColumn(); ImGui::TextUnformatted(hypothesis_status_text(h.status));
            ImGui::TableNextColumn(); ImGui::Text("%.3f", h.confidence);
            ImGui::TableNextColumn(); ImGui::TextWrapped("%s", h.claim.c_str());
        }
        ImGui::EndTable();
    }

    if (ImGui::BeginTable("experiment_table", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("Hypothesis");
        ImGui::TableSetupColumn("Status");
        ImGui::TableSetupColumn("Trials");
        ImGui::TableSetupColumn("Budget");
        ImGui::TableHeadersRow();
        for (const auto& experiment : data.experiments) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::TextUnformatted(format_id(experiment.experiment_id).c_str());
            ImGui::TableNextColumn(); ImGui::TextUnformatted(format_id(experiment.hypothesis_id).c_str());
            ImGui::TableNextColumn(); ImGui::TextUnformatted(experiment_status_text(experiment.status));
            ImGui::TableNextColumn(); ImGui::Text("%llu", static_cast<unsigned long long>(experiment.trial_count));
            ImGui::TableNextColumn(); ImGui::Text("%llu", static_cast<unsigned long long>(experiment.trial_budget));
        }
        ImGui::EndTable();
    }

    ImGui::End();
}

} // namespace xauusd::ui
