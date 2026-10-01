#pragma once

#include "EntityId.h"
#include "OOSWindow.h"
#include "StressTestConfig.h"
#include "ValidationProtocol.h"
#include "ValidationResult.h"
#include "WalkForwardConfig.h"

#include <cstddef>
#include <vector>

namespace xauusd::sovereign {

class IValidationRunner {
public:
    virtual ~IValidationRunner() = default;

    virtual ValidationResult run_oos(const EntityId& candidate_id,
                                     const OOSWindow& window) = 0;

    virtual std::vector<ValidationResult> run_walk_forward(
        const EntityId& candidate_id,
        const WalkForwardConfig& config) = 0;

    virtual ValidationResult run_stress(const EntityId& candidate_id,
                                        const StressTestConfig& config) = 0;

    virtual std::size_t total_runs() const = 0;
};

} // namespace xauusd::sovereign
