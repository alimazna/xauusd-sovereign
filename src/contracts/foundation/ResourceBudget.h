#pragma once
#include "ResourceType.h"
#include <cstdint>
#include <map>
#include <utility>
namespace xauusd::sovereign {
struct ResourceBudget {
    std::map<ResourceType, std::uint64_t> limits;
    std::uint64_t max_runtime_hours{0};
    std::uint64_t max_experiments{0};
    std::uint64_t max_trials{0};
    ResourceBudget() = default;
    ResourceBudget(std::map<ResourceType, std::uint64_t> limits,
                   std::uint64_t max_runtime_hours,
                   std::uint64_t max_experiments,
                   std::uint64_t max_trials)
        : limits(std::move(limits)), max_runtime_hours(max_runtime_hours),
          max_experiments(max_experiments), max_trials(max_trials) {}
};
}
