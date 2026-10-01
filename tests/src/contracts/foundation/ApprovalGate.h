#pragma once
#include "IApprovalGate.h"
#include <array>
#include <cstdint>
#include <map>
#include <set>
#include <vector>
namespace xauusd::sovereign {
class ApprovalGate final : public IApprovalGate {
public:
    ApprovalGate() = default;
    bool submit(const ApprovalRequest& request) override;
    bool decide(const ApprovalDecision& decision) override;
    bool get_request(const EntityId& request_id, ApprovalRequest& out) const override;
    std::vector<ApprovalRecord> all_records() const override;
    std::size_t pending_count() const override;
    std::size_t record_count() const override;
    void clear();
private:
    std::vector<ApprovalRequest> requests_;
    std::map<std::array<std::uint8_t, 16>, std::size_t> request_index_;
    std::vector<ApprovalRecord> records_;
    std::set<std::array<std::uint8_t, 16>> decided_;
};
}
