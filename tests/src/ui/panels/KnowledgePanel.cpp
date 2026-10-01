#include "KnowledgePanel.h"

#include <imgui.h>

namespace xauusd::ui {

namespace {
const char* status_text(xauusd::sovereign::KnowledgeStatus value) {
    switch (value) {
        case xauusd::sovereign::KnowledgeStatus::OBSERVED: return "OBSERVED";
        case xauusd::sovereign::KnowledgeStatus::SUSPECTED: return "SUSPECTED";
        case xauusd::sovereign::KnowledgeStatus::UNDER_INVESTIGATION: return "UNDER_INVESTIGATION";
        case xauusd::sovereign::KnowledgeStatus::SUPPORTED: return "SUPPORTED";
        case xauusd::sovereign::KnowledgeStatus::VALIDATED: return "VALIDATED";
        case xauusd::sovereign::KnowledgeStatus::OPERATIONAL_KNOWLEDGE: return "OPERATIONAL_KNOWLEDGE";
        case xauusd::sovereign::KnowledgeStatus::REFUTED: return "REFUTED";
        case xauusd::sovereign::KnowledgeStatus::CONTRADICTED: return "CONTRADICTED";
        case xauusd::sovereign::KnowledgeStatus::AGING: return "AGING";
        default: return "UNKNOWN";
    }
}
}

void KnowledgePanel::render(UiState& state, const LearningAdapter& learning) {
    if (!ImGui::Begin("Knowledge", &state.panels.knowledge)) {
        ImGui::End();
        return;
    }

    const auto data = learning.snapshot();
    ImGui::Text("Knowledge objects: %llu  |  Failure memories: %llu",
                static_cast<unsigned long long>(data.knowledge.size()),
                static_cast<unsigned long long>(data.failures.size()));

    if (ImGui::BeginTable("knowledge_table", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp)) {
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("Status");
        ImGui::TableSetupColumn("Confidence");
        ImGui::TableSetupColumn("Observation");
        ImGui::TableSetupColumn("Conclusion");
        ImGui::TableHeadersRow();
        for (const auto& object : data.knowledge) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::TextUnformatted(format_id(object.knowledge_id.entity_id()).c_str());
            ImGui::TableNextColumn(); ImGui::TextUnformatted(status_text(object.status));
            ImGui::TableNextColumn(); ImGui::Text("%.3f", object.confidence);
            ImGui::TableNextColumn(); ImGui::TextWrapped("%s", object.observation.c_str());
            ImGui::TableNextColumn(); ImGui::TextWrapped("%s", object.conclusion.c_str());
        }
        ImGui::EndTable();
    }

    ImGui::End();
}

} // namespace xauusd::ui
