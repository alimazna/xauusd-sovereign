#include "SchedulePanel.h"

#include <imgui.h>

namespace xauusd::ui {

namespace {
const char* mode_text(xauusd::sovereign::OperatingMode value) {
    switch (value) {
        case xauusd::sovereign::OperatingMode::OFFLINE: return "OFFLINE";
        case xauusd::sovereign::OperatingMode::SCHEDULED: return "SCHEDULED";
        case xauusd::sovereign::OperatingMode::STARTING: return "STARTING";
        case xauusd::sovereign::OperatingMode::ACTIVE: return "ACTIVE";
        case xauusd::sovereign::OperatingMode::PAUSED: return "PAUSED";
        case xauusd::sovereign::OperatingMode::DRAINING: return "DRAINING";
        case xauusd::sovereign::OperatingMode::SAFE_SHUTDOWN: return "SAFE_SHUTDOWN";
        case xauusd::sovereign::OperatingMode::EMERGENCY_STOP: return "EMERGENCY_STOP";
        default: return "UNKNOWN";
    }
}
const char* checkpoint_status_text(xauusd::sovereign::CheckpointStatus value) {
    switch (value) {
        case xauusd::sovereign::CheckpointStatus::CREATED: return "CREATED";
        case xauusd::sovereign::CheckpointStatus::VALIDATED: return "VALIDATED";
        case xauusd::sovereign::CheckpointStatus::RESTORED: return "RESTORED";
        case xauusd::sovereign::CheckpointStatus::INVALID: return "INVALID";
        case xauusd::sovereign::CheckpointStatus::EXPIRED: return "EXPIRED";
        default: return "UNKNOWN";
    }
}
}

void SchedulePanel::render(UiState& state, const ScheduleAdapter& schedule) {
    if (!ImGui::Begin("Schedule", &state.panels.schedule)) {
        ImGui::End();
        return;
    }

    const auto data = schedule.snapshot();
    ImGui::Text("Mode: %s  |  Schedule events: %llu  |  Checkpoints: %llu",
                mode_text(data.mode),
                static_cast<unsigned long long>(data.events.size()),
                static_cast<unsigned long long>(data.checkpoints.size()));

    ImGui::TextWrapped("The current ScheduleManager contract exposes current_mode() and event history, but not the configured OperatingSchedule windows. Window data is therefore not synthesized.");

    if (ImGui::BeginTable("schedule_events", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("Time");
        ImGui::TableSetupColumn("From");
        ImGui::TableSetupColumn("To");
        ImGui::TableSetupColumn("Reason");
        ImGui::TableHeadersRow();
        for (const auto& event : data.events) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::TextUnformatted(format_timestamp(event.event_time).c_str());
            ImGui::TableNextColumn(); ImGui::TextUnformatted(mode_text(event.from_mode));
            ImGui::TableNextColumn(); ImGui::TextUnformatted(mode_text(event.to_mode));
            ImGui::TableNextColumn(); ImGui::TextUnformatted(event.reason.c_str());
        }
        ImGui::EndTable();
    }

    if (ImGui::BeginTable("schedule_checkpoints", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("Checkpoint");
        ImGui::TableSetupColumn("Status");
        ImGui::TableSetupColumn("Progress");
        ImGui::TableSetupColumn("Description");
        ImGui::TableHeadersRow();
        for (const auto& checkpoint : data.checkpoints) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::TextUnformatted(format_id(checkpoint.metadata.checkpoint_id).c_str());
            ImGui::TableNextColumn(); ImGui::TextUnformatted(checkpoint_status_text(checkpoint.status));
            ImGui::TableNextColumn(); ImGui::Text("%llu%%", static_cast<unsigned long long>(checkpoint.progress_percent));
            ImGui::TableNextColumn(); ImGui::TextUnformatted(checkpoint.metadata.description.c_str());
        }
        ImGui::EndTable();
    }

    ImGui::End();
}

} // namespace xauusd::ui
