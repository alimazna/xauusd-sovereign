#pragma once

#include "CanaryStage.h"
#include "EntityId.h"
#include "Timestamp.h"

#include <string>
#include <utility>

namespace xauusd::sovereign {

struct ScaleUpRequest {
    EntityId    request_id;
    CanaryStage from_stage;
    CanaryStage to_stage;
    std::string justification;
    Timestamp   requested_at;

    ScaleUpRequest() = default;

    ScaleUpRequest(EntityId request_id, CanaryStage from_stage,
                   CanaryStage to_stage, std::string justification,
                   Timestamp requested_at)
        : request_id(request_id), from_stage(from_stage),
          to_stage(to_stage), justification(std::move(justification)),
          requested_at(requested_at) {}
};

} // namespace xauusd::sovereign
