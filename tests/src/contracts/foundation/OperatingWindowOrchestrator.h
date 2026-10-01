#pragma once
#include "CheckpointStore.h"
#include "CrashRecoveryManager.h"
#include "ResourceGovernor.h"
#include "ScheduleManager.h"
#include <cstddef>
namespace xauusd::sovereign {
class OperatingWindowOrchestrator {
public:
    OperatingWindowOrchestrator() = default;

    bool configure_schedule(const OperatingSchedule& schedule);
    OperatingMode current_mode() const;
    bool transition_mode(OperatingMode new_mode, Timestamp at, const std::string& reason);

    bool store_checkpoint(const Checkpoint& checkpoint);
    bool update_checkpoint_status(const EntityId& checkpoint_id, CheckpointStatus status);
    std::size_t checkpoint_count() const;

    void set_resource_budget(const ResourceBudget& budget);
    bool consume_resource(ResourceType type, std::uint64_t amount);
    bool within_budget() const;

    bool plan_recovery(const RecoveryPlan& plan);
    bool record_recovery_report(const RecoveryReport& report);
    std::size_t recovery_plan_count() const;

    void clear();

private:
    ScheduleManager       schedule_;
    CheckpointStore       checkpoints_;
    ResourceGovernor      resources_;
    CrashRecoveryManager  recovery_;
};
}
