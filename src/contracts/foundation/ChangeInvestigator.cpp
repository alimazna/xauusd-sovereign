#include "ChangeInvestigator.h"
namespace xauusd::sovereign {
bool ChangeInvestigator::submit(const ChangeProposal& proposal) {
    const auto key = proposal.proposal_id.bytes();
    if (index_.find(key) != index_.end()) {
        return false;
    }
    proposals_.push_back(proposal);
    index_.emplace(key, proposals_.size() - 1U);
    return true;
}

bool ChangeInvestigator::update_status(const EntityId& proposal_id, ChangeProposalStatus status) {
    const auto it = index_.find(proposal_id.bytes());
    if (it == index_.end()) {
        return false;
    }
    proposals_[it->second].status = status;
    return true;
}

bool ChangeInvestigator::get(const EntityId& proposal_id, ChangeProposal& out) const {
    const auto it = index_.find(proposal_id.bytes());
    if (it == index_.end()) {
        return false;
    }
    out = proposals_[it->second];
    return true;
}

bool ChangeInvestigator::attach_report(const InvestigationReport& report) {
    const auto proposal_key = report.proposal_id.bytes();
    if (index_.find(proposal_key) == index_.end() || reports_.find(proposal_key) != reports_.end()) {
        return false;
    }
    reports_.emplace(proposal_key, report);
    return true;
}

InvestigationReport ChangeInvestigator::report_for(const EntityId& proposal_id) const {
    const auto it = reports_.find(proposal_id.bytes());
    if (it == reports_.end()) {
        return InvestigationReport{};
    }
    return it->second;
}

std::vector<ChangeProposal> ChangeInvestigator::all() const { return proposals_; }
std::size_t ChangeInvestigator::size() const { return proposals_.size(); }

void ChangeInvestigator::clear() {
    proposals_.clear();
    index_.clear();
    reports_.clear();
}
} // namespace xauusd::sovereign
