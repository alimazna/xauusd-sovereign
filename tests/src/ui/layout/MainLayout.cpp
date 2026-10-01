#include "MainLayout.h"

#include <imgui.h>

namespace xauusd::ui {

MainLayout::MainLayout()
    : runtime_(nullptr),
      observation_(nullptr, nullptr, nullptr),
      learning_(nullptr, nullptr),
      research_(nullptr, nullptr),
      evolution_(nullptr),
      validation_(nullptr),
      governance_(nullptr, nullptr),
      schedule_(nullptr, nullptr) {}

void MainLayout::render(UiState& state) {
    menu_bar_.render(state);

    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    if (state.panels.dashboard) {
        dashboard_.render(state, runtime_, observation_, governance_);
    }
    if (state.panels.market) {
        market_.render(state);
    }
    if (state.panels.predictions) {
        predictions_.render(state, observation_);
    }
    if (state.panels.approval) {
        approval_.render(state, governance_);
    }
    if (state.panels.incidents) {
        incidents_.render(state, governance_);
    }
    if (state.panels.research) {
        research_panel_.render(state, research_);
    }
    if (state.panels.knowledge) {
        knowledge_panel_.render(state, learning_);
    }
    if (state.panels.candidates) {
        candidate_panel_.render(state, evolution_);
    }
    if (state.panels.validation) {
        validation_panel_.render(state, validation_);
    }
    if (state.panels.schedule) {
        schedule_panel_.render(state, schedule_);
    }

    if (state.panels.about) {
        if (ImGui::Begin("About", &state.panels.about)) {
            ImGui::TextUnformatted("XAUUSD Sovereign — Desktop Control Center");
            ImGui::TextUnformatted("Phase 9 Dear ImGui prototype");
            ImGui::TextUnformatted("UI stack: Dear ImGui + GLFW + OpenGL 3.3 + C++20");
            ImGui::TextUnformatted("No foundation mutation, networking, persistence, or threads are performed by this UI layer.");
        }
        ImGui::End();
    }

    status_bar_.render(state, runtime_, observation_, governance_);
    notifications_.render(state);
}

void MainLayout::bind(
    const xauusd::sovereign::RuntimeEngine* runtime,
    const xauusd::sovereign::PredictionLedger* predictions,
    const xauusd::sovereign::OutcomeEngine* outcomes,
    const xauusd::sovereign::FailureDetector* failures,
    const xauusd::sovereign::KnowledgeStore* knowledge,
    const xauusd::sovereign::FailureMemory* failure_memory,
    const xauusd::sovereign::HypothesisStore* hypotheses,
    const xauusd::sovereign::ExperimentLedger* experiments,
    const xauusd::sovereign::CandidateRegistry* candidates,
    const xauusd::sovereign::ValidationFirewall* validation,
    const xauusd::sovereign::ApprovalGate* approvals,
    const xauusd::sovereign::IncidentTracker* incidents,
    const xauusd::sovereign::ScheduleManager* schedule,
    const xauusd::sovereign::CheckpointStore* checkpoints) noexcept {
    runtime_.bind(runtime);
    observation_.bind(predictions, outcomes, failures);
    learning_.bind(knowledge, failure_memory);
    research_.bind(hypotheses, experiments);
    evolution_.bind(candidates);
    validation_.bind(validation);
    governance_.bind(approvals, incidents);
    schedule_.bind(schedule, checkpoints);
}

} // namespace xauusd::ui
