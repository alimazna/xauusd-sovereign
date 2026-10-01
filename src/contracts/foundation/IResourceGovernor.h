#pragma once
#include "ResourceBudget.h"
#include "ResourceType.h"
#include "ResourceUsage.h"
#include <cstdint>
namespace xauusd::sovereign {
class IResourceGovernor {
public:
    virtual ~IResourceGovernor() = default;
    virtual void set_budget(const ResourceBudget& budget) = 0;
    virtual bool consume(ResourceType type, std::uint64_t amount) = 0;
    virtual bool consume_runtime(std::uint64_t hours) = 0;
    virtual bool consume_experiment() = 0;
    virtual bool consume_trial() = 0;
    virtual ResourceUsage current_usage() const = 0;
    virtual bool within_budget() const = 0;
};
}
