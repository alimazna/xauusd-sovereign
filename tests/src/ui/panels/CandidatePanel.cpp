#include "CandidatePanel.h"

#include <imgui.h>

namespace xauusd::ui {

namespace {
const char* status_text(xauusd::sovereign::CandidateStatus value) {
    switch (value) {
        case xauusd::sovereign::CandidateStatus::DRAFTED: return "DRAFTED";
        case xauusd::sovereign::CandidateStatus::SANDBOXED: return "SANDBOXED";
        case xauusd::sovereign::CandidateStatus::VALIDATING: return "VALIDATING";
        case xauusd::sovereign::CandidateStatus::PASSED: return "PASSED";
        case xauusd::sovereign::CandidateStatus::FAILED: return "FAILED";
        case xauusd::sovereign::CandidateStatus::INCONCLUSIVE: return "INCONCLUSIVE";
        case xauusd::sovereign::CandidateStatus::CONTAMINATED: return "CONTAMINATED";
        case xauusd::sovereign::CandidateStatus::REVIEW_READY: return "REVIEW_READY";
        case xauusd::sovereign::CandidateStatus::HUMAN_APPROVED: return "HUMAN_APPROVED";
        case xauusd::sovereign::CandidateStatus::HUMAN_REJECTED: return "HUMAN_REJECTED";
        case xauusd::sovereign::CandidateStatus::SHADOW: return "SHADOW";
        case xauusd::sovereign::CandidateStatus::PROMOTION_READY: return "PROMOTION_READY";
        case xauusd::sovereign::CandidateStatus::DEPLOYED: return "DEPLOYED";
        case xauusd::sovereign::CandidateStatus::ROLLED_BACK: return "ROLLED_BACK";
        case xauusd::sovereign::CandidateStatus::RETIRED: return "RETIRED";
        default: return "UNKNOWN";
    }
}
const char* type_text(xauusd::sovereign::CandidateType value) {
    switch (value) {
        case xauusd::sovereign::CandidateType::PARAMETER_CANDIDATE: return "PARAMETER";
        case xauusd::sovereign::CandidateType::RULE_CANDIDATE: return "RULE";
        case xauusd::sovereign::CandidateType::FEATURE_CANDIDATE: return "FEATURE";
        case xauusd::sovereign::CandidateType::REGIME_CANDIDATE: return "REGIME";
        case xauusd::sovereign::CandidateType::STRATEGY_CANDIDATE: return "STRATEGY";
        case xauusd::sovereign::CandidateType::RESEARCH_METHOD_CANDIDATE: return "RESEARCH_METHOD";
        case xauusd::sovereign::CandidateType::ARCHITECTURE_CANDIDATE: return "ARCHITECTURE";
        case xauusd::sovereign::CandidateType::UNKNOWN_CANDIDATE: return "UNKNOWN";
        default: return "UNKNOWN";
    }
}
}

void CandidatePanel::render(UiState& state, const EvolutionAdapter& evolution) {
    if (!ImGui::Begin("Candidates", &state.panels.candidates)) {
        ImGui::End();
        return;
    }

    const auto candidates = evolution.snapshot();
    ImGui::Text("Candidates: %llu", static_cast<unsigned long long>(candidates.size()));

    if (ImGui::BeginTable("candidate_table", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp)) {
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("Type");
        ImGui::TableSetupColumn("Status");
        ImGui::TableSetupColumn("Confidence");
        ImGui::TableSetupColumn("Trials");
        ImGui::TableSetupColumn("Change summary");
        ImGui::TableHeadersRow();
        for (const auto& candidate : candidates) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            const auto id = format_id(candidate.candidate_id);
            if (ImGui::Selectable(id.c_str(), state.selection.selected_candidate == candidate.candidate_id)) {
                state.selection.selected_candidate = candidate.candidate_id;
            }
            ImGui::TableNextColumn(); ImGui::TextUnformatted(type_text(candidate.candidate_type));
            ImGui::TableNextColumn(); ImGui::TextUnformatted(status_text(candidate.status));
            ImGui::TableNextColumn(); ImGui::Text("%.3f", candidate.confidence);
            ImGui::TableNextColumn(); ImGui::Text("%llu", static_cast<unsigned long long>(candidate.trial_count));
            ImGui::TableNextColumn(); ImGui::TextWrapped("%s", candidate.change_summary.c_str());
        }
        ImGui::EndTable();
    }

    ImGui::End();
}

} // namespace xauusd::ui
