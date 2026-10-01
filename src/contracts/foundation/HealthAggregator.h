#pragma once

#include "IHealthAggregator.h"
#include "HealthSnapshot.h"
#include "FailurePattern.h"

#include <array>
#include <cstdint>
#include <map>
#include <vector>

namespace xauusd::sovereign {

class HealthAggregator final : public IHealthAggregator {
public:
    HealthAggregator() = default;

    void observe_snapshot(const HealthSnapshot& snapshot) override;

    void observe_failure(const FailurePattern& pattern) override;

    SystemHealthSnapshot current_health() const override;

    HealthAggregate aggregate() const override;

    void clear();

private:
    std::map<std::array<std::uint8_t, 16>, HealthSnapshot> snapshots_;
    std::map<std::array<std::uint8_t, 16>, FailurePattern> failures_;
};

} // namespace xauusd::sovereign
