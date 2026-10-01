#include "PredictionPanel.h"

#include <imgui.h>

namespace xauusd::ui {

namespace {
const char* direction_text(xauusd::sovereign::PredictionDirection value) {
    switch (value) {
        case xauusd::sovereign::PredictionDirection::UP: return "UP";
        case xauusd::sovereign::PredictionDirection::DOWN: return "DOWN";
        case xauusd::sovereign::PredictionDirection::NEUTRAL: return "NEUTRAL";
        default: return "UNKNOWN";
    }
}
}

void PredictionPanel::render(UiState& state, const ObservationAdapter& observation) {
    if (!ImGui::Begin("Predictions", &state.panels.predictions)) {
        ImGui::End();
        return;
    }

    const auto data = observation.snapshot();
    ImGui::Text("Predictions: %llu  |  Outcomes: %llu",
                static_cast<unsigned long long>(data.predictions.size()),
                static_cast<unsigned long long>(data.outcomes.size()));

    if (data.predictions.empty()) {
        ImGui::TextUnformatted("No prediction records available.");
        ImGui::End();
        return;
    }

    if (ImGui::BeginTable("prediction_table", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp)) {
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("Direction");
        ImGui::TableSetupColumn("Timeframe");
        ImGui::TableSetupColumn("Strategy");
        ImGui::TableSetupColumn("Validation");
        ImGui::TableSetupColumn("Notes");
        ImGui::TableHeadersRow();

        for (const auto& record : data.predictions) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            const auto id = format_id(record.record_id);
            if (ImGui::Selectable(id.c_str(), state.selection.selected_prediction == record.record_id, ImGuiSelectableFlags_SpanAllColumns)) {
                state.selection.selected_prediction = record.record_id;
            }
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(direction_text(record.prediction.direction));
            ImGui::TableNextColumn();
            ImGui::Text("%u", static_cast<unsigned int>(record.prediction.trigger_timeframe));
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(record.prediction.strategy_name.c_str());
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(record.validation.passed ? "PASSED" : "NOT PASSED");
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(record.notes.c_str());
        }
        ImGui::EndTable();
    }

    ImGui::End();
}

} // namespace xauusd::ui
