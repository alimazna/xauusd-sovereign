#pragma once
#include "ResourceType.h"
#include <cstdint>
#include <map>
#include <utility>
namespace xauusd::sovereign {
struct ResourceUsage {
    std::map<ResourceType, std::uint64_t> used;
    std::uint64_t runtime_hours_used{0};
    std::uint64_t experiments_used{0};
    std::uint64_t trials_used{0};
    ResourceUsage() = default;
    ResourceUsage(std::map<ResourceType, std::uint64_t> used,
                  std::uint64_t runtime_hours_used,
                  std::uint64_t experiments_used,
                  std::uint64_t trials_used)
        : used(std::move(used)), runtime_hours_used(runtime_hours_used),
          experiments_used(experiments_used), trials_used(trials_used) {}
};
}
