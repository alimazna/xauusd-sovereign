#pragma once

#include "CanaryConfig.h"
#include "CanaryStage.h"
#include "EntityId.h"
#include "ScaleUpDecision.h"
#include "ScaleUpRequest.h"

#include <vector>

namespace xauusd::sovereign {

class ICanaryController {
public:
    virtual ~ICanaryController() = default;

    virtual bool configure(const CanaryConfig& config) = 0;
    virtual CanaryStage current_stage() const = 0;
    virtual bool submit_scale_up(const ScaleUpRequest& request) = 0;
    virtual bool decide_scale_up(const ScaleUpDecision& decision) = 0;
    virtual std::vector<ScaleUpDecision> all_decisions() const = 0;
};

} // namespace xauusd::sovereign
