#include "GovernanceOrchestrator.h"
namespace xauusd::sovereign {
bool GovernanceOrchestrator::submit_approval(const ApprovalRequest& request) { return approvals_.submit(request); }
bool GovernanceOrchestrator::decide_approval(const ApprovalDecision& decision) { return approvals_.decide(decision); }
std::size_t GovernanceOrchestrator::pending_approval_count() const { return approvals_.pending_count(); }
bool GovernanceOrchestrator::submit_proposal(const ChangeProposal& proposal) { return investigations_.submit(proposal); }
bool GovernanceOrchestrator::update_proposal_status(const EntityId& proposal_id, ChangeProposalStatus status) { return investigations_.update_status(proposal_id, status); }
bool GovernanceOrchestrator::attach_report(const InvestigationReport& report) { return investigations_.attach_report(report); }
bool GovernanceOrchestrator::submit_promotion(const PromotionPackage& package) { return promotions_.submit(package); }
bool GovernanceOrchestrator::update_promotion_status(const EntityId& package_id, PromotionStatus status) { return promotions_.update_status(package_id, status); }
bool GovernanceOrchestrator::register_known_good(const KnownGoodVersion& entry) { return known_good_.register_version(entry); }
bool GovernanceOrchestrator::record_incident(const Incident& incident) { return incidents_.record(incident); }
bool GovernanceOrchestrator::resolve_incident(const EntityId& incident_id) { return incidents_.resolve(incident_id); }
bool GovernanceOrchestrator::submit_rollback(const RollbackRequest& request) { return rollbacks_.submit(request); }
bool GovernanceOrchestrator::record_rollback_result(const RollbackResult& result) { return rollbacks_.record_result(result); }
void GovernanceOrchestrator::clear() {
    approvals_.clear();
    investigations_.clear();
    promotions_.clear();
    known_good_.clear();
    rollbacks_.clear();
    incidents_.clear();
}
} // namespace xauusd::sovereign
