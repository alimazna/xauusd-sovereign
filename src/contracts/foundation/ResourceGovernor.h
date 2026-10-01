#pragma once
#include "IResourceGovernor.h"
namespace xauusd::sovereign {
class ResourceGovernor final : public IResourceGovernor {
public:
    ResourceGovernor() = default;
    void set_budget(const ResourceBudget& budget) override;
    bool consume(ResourceType type, std::uint64_t amount) override;
    bool consume_runtime(std::uint64_t hours) override;
    bool consume_experiment() override;
    bool consume_trial() override;
    ResourceUsage current_usage() const override;
    bool within_budget() const override;
    void clear();
private:
    ResourceBudget budget_;
    ResourceUsage usage_;
};
}
