#pragma once

#include "Timestamp.h"
#include "HealthSnapshot.h"
#include "FailurePattern.h"
#include "SystemHealthLevel.h"

#include <string>
#include <utility>
#include <vector>

namespace xauusd::sovereign {

class HealthAggregate {
public:
    Timestamp                    generated_at;
    SystemHealthLevel            overall_level;
    std::vector<HealthSnapshot>  services;
    std::vector<FailurePattern>  recent_failures;
    std::string                  notes;

    HealthAggregate() = default;

    HealthAggregate(
        Timestamp generated_at,
        SystemHealthLevel overall_level,
        std::vector<HealthSnapshot> services,
        std::vector<FailurePattern> recent_failures,
        std::string notes)
        : generated_at(generated_at),
          overall_level(overall_level),
          services(std::move(services)),
          recent_failures(std::move(recent_failures)),
          notes(std::move(notes)) {}
};

} // namespace xauusd::sovereign
