#pragma once
#include "Candidate.h"
#include "EntityId.h"
#include "Timestamp.h"
#include "Version.h"
#include <string>
#include <utility>
namespace xauusd::sovereign {
struct ApprovalRequest {
    EntityId     request_id;
    EntityId     candidate_id;
    EntityId     proposal_id;
    Version      current_version;
    Version      candidate_version;
    std::string  summary;
    Timestamp    created_at;
    ApprovalRequest() = default;
    ApprovalRequest(EntityId request_id, EntityId candidate_id,
                    EntityId proposal_id, Version current_version,
                    Version candidate_version, std::string summary,
                    Timestamp created_at)
        : request_id(request_id), candidate_id(candidate_id), proposal_id(proposal_id),
          current_version(current_version), candidate_version(candidate_version),
          summary(std::move(summary)), created_at(created_at) {}
};
}
