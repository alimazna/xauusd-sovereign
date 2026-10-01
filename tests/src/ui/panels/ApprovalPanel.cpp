#include "ApprovalPanel.h"

#include <imgui.h>

namespace xauusd::ui {

namespace {
const char* status_text(xauusd::sovereign::ApprovalStatus value) {
    switch (value) {
        case xauusd::sovereign::ApprovalStatus::PENDING: return "PENDING";
        case xauusd::sovereign::ApprovalStatus::APPROVED: return "APPROVED";
        case xauusd::sovereign::ApprovalStatus::REJECTED: return "REJECTED";
        case xauusd::sovereign::ApprovalStatus::REQUEST_MORE_RESEARCH: return "MORE_RESEARCH";
        case xauusd::sovereign::ApprovalStatus::MODIFY_PROPOSAL: return "MODIFY";
        case xauusd::sovereign::ApprovalStatus::FROZEN: return "FROZEN";
        case xauusd::sovereign::ApprovalStatus::EXPIRED: return "EXPIRED";
        default: return "UNKNOWN";
    }
}
}

void ApprovalPanel::render(UiState& state, const GovernanceAdapter& governance) {
    if (!ImGui::Begin("Approvals", &state.panels.approval)) {
        ImGui::End();
        return;
    }

    const auto data = governance.snapshot();
    ImGui::Text("Pending approvals: %llu", static_cast<unsigned long long>(data.pending_approvals));
    ImGui::Text("Decision records: %llu", static_cast<unsigned long long>(data.approval_records));
    ImGui::TextWrapped("The foundation contract exposes pending_count() but does not expose pending request enumeration. Decision dispatch is intentionally read-only here.");

    if (ImGui::BeginTable("approval_table", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("Record");
        ImGui::TableSetupColumn("Candidate");
        ImGui::TableSetupColumn("Status");
        ImGui::TableSetupColumn("Reason");
        ImGui::TableSetupColumn("Actions");
        ImGui::TableHeadersRow();

        for (const auto& record : data.approval_history) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            const auto id = format_id(record.record_id);
            if (ImGui::Selectable(id.c_str(), state.selection.selected_approval == record.record_id)) {
                state.selection.selected_approval = record.record_id;
            }
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(format_id(record.request.candidate_id).c_str());
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(status_text(record.decision.status));
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(record.decision.reason.c_str());
            ImGui::TableNextColumn();
            if (ImGui::Button(("Approve##" + id).c_str())) {
                set_notification(state, "Read-only UI: approval commands require a governance command bridge.");
            }
            ImGui::SameLine();
            if (ImGui::Button(("Reject##" + id).c_str())) {
                set_notification(state, "Read-only UI: approval commands require a governance command bridge.");
            }
        }
        ImGui::EndTable();
    }

    ImGui::End();
}

} // namespace xauusd::ui
