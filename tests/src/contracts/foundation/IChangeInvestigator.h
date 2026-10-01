#pragma once
#include "ChangeProposal.h"
#include "EntityId.h"
#include "InvestigationReport.h"
#include <cstddef>
#include <vector>
namespace xauusd::sovereign {
class IChangeInvestigator {
public:
    virtual ~IChangeInvestigator() = default;
    virtual bool submit(const ChangeProposal& proposal) = 0;
    virtual bool update_status(const EntityId& proposal_id,
                               ChangeProposalStatus status) = 0;
    virtual bool get(const EntityId& proposal_id,
                     ChangeProposal& out) const = 0;
    virtual bool attach_report(const InvestigationReport& report) = 0;
    virtual InvestigationReport report_for(const EntityId& proposal_id) const = 0;
    virtual std::vector<ChangeProposal> all() const = 0;
    virtual std::size_t size() const = 0;
};
}
