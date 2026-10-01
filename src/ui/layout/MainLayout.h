#pragma once

#include "../models/UiState.h"
#include "../adapters/RuntimeAdapter.h"
#include "../adapters/ObservationAdapter.h"
#include "../adapters/LearningAdapter.h"
#include "../adapters/ResearchAdapter.h"
#include "../adapters/EvolutionAdapter.h"
#include "../adapters/ValidationAdapter.h"
#include "../adapters/GovernanceAdapter.h"
#include "../adapters/ScheduleAdapter.h"
#include "../panels/DashboardPanel.h"
#include "../panels/MarketPanel.h"
#include "../panels/PredictionPanel.h"
#include "../panels/ApprovalPanel.h"
#include "../panels/IncidentPanel.h"
#include "../panels/ResearchPanel.h"
#include "../panels/KnowledgePanel.h"
#include "../panels/CandidatePanel.h"
#include "../panels/ValidationPanel.h"
#include "../panels/SchedulePanel.h"
#include "MenuBar.h"
#include "StatusBar.h"
#include "Notifications.h"

namespace xauusd::ui {

class MainLayout {
public:
    MainLayout();
    void render(UiState& state);

    void bind(
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
        const xauusd::sovereign::CheckpointStore* checkpoints) noexcept;

private:
    RuntimeAdapter runtime_;
    ObservationAdapter observation_;
    LearningAdapter learning_;
    ResearchAdapter research_;
    EvolutionAdapter evolution_;
    ValidationAdapter validation_;
    GovernanceAdapter governance_;
    ScheduleAdapter schedule_;

    MenuBar menu_bar_;
    StatusBar status_bar_;
    Notifications notifications_;

    DashboardPanel dashboard_;
    MarketPanel market_;
    PredictionPanel predictions_;
    ApprovalPanel approval_;
    IncidentPanel incidents_;
    ResearchPanel research_panel_;
    KnowledgePanel knowledge_panel_;
    CandidatePanel candidate_panel_;
    ValidationPanel validation_panel_;
    SchedulePanel schedule_panel_;
};

} // namespace xauusd::ui
