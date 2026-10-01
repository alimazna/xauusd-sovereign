#pragma once
#include "ICrashRecoveryManager.h"
#include <array>
#include <cstdint>
#include <map>
#include <vector>
namespace xauusd::sovereign {
class CrashRecoveryManager final : public ICrashRecoveryManager {
public:
    CrashRecoveryManager() = default;
    bool plan(const RecoveryPlan& plan) override;
    bool record_report(const RecoveryReport& report) override;
    bool contains(const EntityId& plan_id) const override;
    bool get_plan(const EntityId& plan_id, RecoveryPlan& out) const override;
    std::vector<RecoveryReport> all_reports() const override;
    std::size_t plan_count() const override;
    void clear();
private:
    std::vector<RecoveryPlan> plans_;
    std::map<std::array<std::uint8_t, 16>, std::size_t> index_;
    std::vector<RecoveryReport> reports_;
};
}
