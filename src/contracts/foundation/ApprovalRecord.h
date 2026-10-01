#pragma once
#include "ApprovalDecision.h"
#include "ApprovalRequest.h"
#include "EntityId.h"
#include <utility>
namespace xauusd::sovereign {
struct ApprovalRecord {
    EntityId           record_id;
    ApprovalRequest    request;
    ApprovalDecision   decision;
    ApprovalRecord() = default;
    ApprovalRecord(EntityId record_id, ApprovalRequest request,
                   ApprovalDecision decision)
        : record_id(record_id), request(std::move(request)),
          decision(std::move(decision)) {}
};
}
