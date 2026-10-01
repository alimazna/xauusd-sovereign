#pragma once
#include "IChangeInvestigator.h"
#include <array>
#include <cstdint>
#include <map>
#include <vector>
namespace xauusd::sovereign {
class ChangeInvestigator final : public IChangeInvestigator {
public:
    ChangeInvestigator() = default;
    bool submit(const ChangeProposal& proposal) override;
    bool update_status(const EntityId& proposal_id, ChangeProposalStatus status) override;
    bool get(const EntityId& proposal_id, ChangeProposal& out) const override;
    bool attach_report(const InvestigationReport& report) override;
    InvestigationReport report_for(const EntityId& proposal_id) const override;
    std::vector<ChangeProposal> all() const override;
    std::size_t size() const override;
    void clear();
private:
    std::vector<ChangeProposal> proposals_;
    std::map<std::array<std::uint8_t, 16>, std::size_t> index_;
    std::map<std::array<std::uint8_t, 16>, InvestigationReport> reports_;
};
}
