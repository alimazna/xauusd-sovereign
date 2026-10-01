#pragma once
#include "ApprovalGate.h"
#include "ChangeInvestigator.h"
#include "IncidentTracker.h"
#include "KnownGoodRegistry.h"
#include "PromotionGate.h"
#include "RollbackManager.h"
#include <cstddef>
namespace xauusd::sovereign {
class GovernanceOrchestrator {
public:
    GovernanceOrchestrator() = default;

    bool submit_approval(const ApprovalRequest& request);
    bool decide_approval(const ApprovalDecision& decision);
    std::size_t pending_approval_count() const;

    bool submit_proposal(const ChangeProposal& proposal);
    bool update_proposal_status(const EntityId& proposal_id, ChangeProposalStatus status);
    bool attach_report(const InvestigationReport& report);

    bool submit_promotion(const PromotionPackage& package);
    bool update_promotion_status(const EntityId& package_id, PromotionStatus status);

    bool register_known_good(const KnownGoodVersion& entry);

    bool record_incident(const Incident& incident);
    bool resolve_incident(const EntityId& incident_id);

    bool submit_rollback(const RollbackRequest& request);
    bool record_rollback_result(const RollbackResult& result);

    void clear();

private:
    ApprovalGate        approvals_;
    ChangeInvestigator  investigations_;
    PromotionGate       promotions_;
    KnownGoodRegistry   known_good_;
    RollbackManager     rollbacks_;
    IncidentTracker     incidents_;
};
}
