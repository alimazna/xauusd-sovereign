#pragma once

#include "HealthSnapshot.h"
#include "FailurePattern.h"
#include "SystemHealthSnapshot.h"
#include "HealthAggregate.h"

namespace xauusd::sovereign {

class IHealthAggregator {
public:
    virtual ~IHealthAggregator() = default;

    virtual void observe_snapshot(const HealthSnapshot& snapshot) = 0;

    virtual void observe_failure(const FailurePattern& pattern) = 0;

    virtual SystemHealthSnapshot current_health() const = 0;

    virtual HealthAggregate aggregate() const = 0;
};

} // namespace xauusd::sovereign
