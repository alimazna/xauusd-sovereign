#pragma once
#include "ApprovalDecision.h"
#include "ApprovalRecord.h"
#include "ApprovalRequest.h"
#include "EntityId.h"
#include <cstddef>
#include <vector>
namespace xauusd::sovereign {
class IApprovalGate {
public:
    virtual ~IApprovalGate() = default;
    virtual bool submit(const ApprovalRequest& request) = 0;
    virtual bool decide(const ApprovalDecision& decision) = 0;
    virtual bool get_request(const EntityId& request_id,
                             ApprovalRequest& out) const = 0;
    virtual std::vector<ApprovalRecord> all_records() const = 0;
    virtual std::size_t pending_count() const = 0;
    virtual std::size_t record_count() const = 0;
};
}
