#include "ValidationPanel.h"

#include <imgui.h>

namespace xauusd::ui {

namespace {
const char* method_text(xauusd::sovereign::ValidationMethod value) {
    switch (value) {
        case xauusd::sovereign::ValidationMethod::TIME_AWARE: return "TIME_AWARE";
        case xauusd::sovereign::ValidationMethod::PURGED_KFOLD: return "PURGED_KFOLD";
        case xauusd::sovereign::ValidationMethod::CPCV: return "CPCV";
        case xauusd::sovereign::ValidationMethod::WALK_FORWARD: return "WALK_FORWARD";
        case xauusd::sovereign::ValidationMethod::MONTE_CARLO_STRESS: return "MONTE_CARLO_STRESS";
        case xauusd::sovereign::ValidationMethod::PBO_CSCV: return "PBO_CSCV";
        case xauusd::sovereign::ValidationMethod::DEFLATED_SHARPE: return "DEFLATED_SHARPE";
        case xauusd::sovereign::ValidationMethod::REALITY_CHECK: return "REALITY_CHECK";
        default: return "UNKNOWN";
    }
}
}

void ValidationPanel::render(UiState& state, const ValidationAdapter& validation) {
    if (!ImGui::Begin("Validation", &state.panels.validation)) {
        ImGui::End();
        return;
    }

    const auto data = validation.snapshot(nullptr);
    ImGui::Text("Protocols: %llu  |  Runs: %llu",
                static_cast<unsigned long long>(data.protocol_count),
                static_cast<unsigned long long>(data.run_count));
    ImGui::TextWrapped("The foundation ValidationFirewall exposes run_count(), but not an all-runs enumeration method. This panel therefore reports counts and renders results when a host integration supplies a selected run through the adapter.");

    if (data.results.empty()) {
        ImGui::TextUnformatted("No selected validation run results available.");
        ImGui::End();
        return;
    }

    if (ImGui::BeginTable("validation_results", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("Result");
        ImGui::TableSetupColumn("Method");
        ImGui::TableSetupColumn("Passed");
        ImGui::TableSetupColumn("Score");
        ImGui::TableSetupColumn("Threshold");
        ImGui::TableSetupColumn("Trials");
        ImGui::TableHeadersRow();
        for (const auto& result : data.results) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::TextUnformatted(format_id(result.result_id).c_str());
            ImGui::TableNextColumn(); ImGui::TextUnformatted(method_text(result.method));
            ImGui::TableNextColumn(); ImGui::TextUnformatted(result.passed ? "YES" : "NO");
            ImGui::TableNextColumn(); ImGui::Text("%.4f", result.score);
            ImGui::TableNextColumn(); ImGui::Text("%.4f", result.threshold);
            ImGui::TableNextColumn(); ImGui::Text("%llu", static_cast<unsigned long long>(result.trials));
        }
        ImGui::EndTable();
    }

    ImGui::End();
}

} // namespace xauusd::ui
