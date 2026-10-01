#pragma once

#include "ApprovalGate.h"
#include "IncidentTracker.h"

#include <vector>

namespace xauusd::ui {

struct GovernanceSourceStatus {
    bool approvals_bound{false};
    bool incidents_bound{false};
};

struct GovernanceSnapshot {
    std::size_t pending_approvals{0};
    std::size_t approval_records{0};
    std::vector<xauusd::sovereign::ApprovalRecord> approval_history;
    std::vector<xauusd::sovereign::Incident> incidents;
    std::vector<xauusd::sovereign::Incident> unresolved_incidents;
};

class GovernanceAdapter {
public:
    GovernanceAdapter(
        const xauusd::sovereign::ApprovalGate* approvals,
        const xauusd::sovereign::IncidentTracker* incidents) noexcept;

    GovernanceSnapshot snapshot() const;
    bool has_sources() const noexcept;
    GovernanceSourceStatus sources() const noexcept;

    void bind(
        const xauusd::sovereign::ApprovalGate* approvals,
        const xauusd::sovereign::IncidentTracker* incidents) noexcept;

private:
    const xauusd::sovereign::ApprovalGate* approvals_{nullptr};
    const xauusd::sovereign::IncidentTracker* incidents_{nullptr};
};

} // namespace xauusd::ui
