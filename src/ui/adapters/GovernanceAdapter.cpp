#include "GovernanceAdapter.h"

namespace xauusd::ui {

GovernanceAdapter::GovernanceAdapter(
    const xauusd::sovereign::ApprovalGate* approvals,
    const xauusd::sovereign::IncidentTracker* incidents) noexcept
    : approvals_(approvals), incidents_(incidents) {}

GovernanceSnapshot GovernanceAdapter::snapshot() const {
    GovernanceSnapshot result;
    if (approvals_ != nullptr) {
        result.pending_approvals = approvals_->pending_count();
        result.approval_records = approvals_->record_count();
        result.approval_history = approvals_->all_records();
    }
    if (incidents_ != nullptr) {
        result.incidents = incidents_->all();
        result.unresolved_incidents = incidents_->unresolved();
    }
    return result;
}

bool GovernanceAdapter::has_sources() const noexcept {
    return approvals_ != nullptr || incidents_ != nullptr;
}

GovernanceSourceStatus GovernanceAdapter::sources() const noexcept {
    return GovernanceSourceStatus{
        approvals_ != nullptr,
        incidents_ != nullptr
    };
}

void GovernanceAdapter::bind(
    const xauusd::sovereign::ApprovalGate* approvals,
    const xauusd::sovereign::IncidentTracker* incidents) noexcept {
    approvals_ = approvals;
    incidents_ = incidents;
}

} // namespace xauusd::ui
