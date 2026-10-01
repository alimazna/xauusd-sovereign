#include "ScheduleManager.h"
#include "CheckpointStore.h"
#include "ResourceGovernor.h"
#include "CrashRecoveryManager.h"
#include "OperatingWindowOrchestrator.h"

#include <array>
#include <cstdint>
#include <map>
#include <vector>

namespace xauusd::sovereign::tests {
namespace {
#define CHECK(expr) do { if (!(expr)) { return false; } } while (false)

static EntityId make_id(std::uint8_t seed) {
    std::array<std::uint8_t, 16> bytes{};
    bytes[0] = seed;
    return EntityId{bytes};
}

static OperatingSchedule make_schedule() {
    return OperatingSchedule(
        std::vector<ScheduleWindow>{ScheduleWindow(Timestamp(1), Timestamp(10), 5, 3, 8, true, true)},
        "UTC", 3, 8);
}

static Checkpoint make_checkpoint(std::uint8_t seed) {
    return Checkpoint(
        CheckpointMetadata(make_id(seed), make_id(seed + 10U), Version(1), Timestamp(5), "checkpoint"),
        CheckpointStatus::CREATED, std::vector<std::uint8_t>{1, 2, 3}, 50);
}

static RecoveryPlan make_plan(std::uint8_t seed) {
    return RecoveryPlan(make_id(seed), make_id(seed + 10U), RecoveryPhase::DETECTED, "crash", Timestamp(7));
}

static ResourceBudget make_budget() {
    return ResourceBudget(
        std::map<ResourceType, std::uint64_t>{{ResourceType::CPU, 10}, {ResourceType::RAM, 20}},
        8, 2, 3);
}

static bool test_1_schedule_configure() {
    ScheduleManager manager;
    CHECK(manager.configure(make_schedule()));
    CHECK(manager.current_mode() == OperatingMode::SCHEDULED);
    return true;
}

static bool test_2_schedule_transition_to() {
    ScheduleManager manager;
    CHECK(manager.configure(make_schedule()));
    CHECK(manager.transition_to(OperatingMode::ACTIVE, Timestamp(3), "activate"));
    CHECK(manager.current_mode() == OperatingMode::ACTIVE);
    CHECK(manager.all_events().front().from_mode == OperatingMode::SCHEDULED);
    CHECK(manager.all_events().front().to_mode == OperatingMode::ACTIVE);
    return true;
}

static bool test_3_schedule_event_count() {
    ScheduleManager manager;
    CHECK(manager.configure(make_schedule()));
    CHECK(manager.transition_to(OperatingMode::ACTIVE, Timestamp(3), "activate"));
    CHECK(manager.transition_to(OperatingMode::DRAINING, Timestamp(4), "drain"));
    CHECK(manager.event_count() == 2);
    return true;
}

static bool test_4_checkpoint_store() {
    CheckpointStore store;
    const auto checkpoint = make_checkpoint(1);
    CHECK(store.store(checkpoint));
    CHECK(store.contains(checkpoint.metadata.checkpoint_id));
    CHECK(store.size() == 1);
    return true;
}

static bool test_5_checkpoint_duplicate_rejection() {
    CheckpointStore store;
    const auto checkpoint = make_checkpoint(1);
    CHECK(store.store(checkpoint));
    CHECK(!store.store(checkpoint));
    return true;
}

static bool test_6_checkpoint_update_status() {
    CheckpointStore store;
    const auto checkpoint = make_checkpoint(1);
    CHECK(store.store(checkpoint));
    CHECK(store.update_status(checkpoint.metadata.checkpoint_id, CheckpointStatus::VALIDATED));
    Checkpoint out;
    CHECK(store.get(checkpoint.metadata.checkpoint_id, out));
    CHECK(out.status == CheckpointStatus::VALIDATED);
    return true;
}

static bool test_7_checkpoint_get() {
    CheckpointStore store;
    const auto checkpoint = make_checkpoint(1);
    CHECK(store.store(checkpoint));
    Checkpoint out;
    CHECK(store.get(checkpoint.metadata.checkpoint_id, out));
    CHECK(out.payload.size() == 3);
    CHECK(out.progress_percent == 50);
    return true;
}

static bool test_8_resource_set_budget() {
    ResourceGovernor governor;
    governor.set_budget(make_budget());
    CHECK(governor.within_budget());
    CHECK(governor.current_usage().runtime_hours_used == 0);
    return true;
}

static bool test_9_resource_consume_within_limits() {
    ResourceGovernor governor;
    governor.set_budget(make_budget());
    CHECK(governor.consume(ResourceType::CPU, 6));
    CHECK(governor.current_usage().used.at(ResourceType::CPU) == 6);
    return true;
}

static bool test_10_resource_exceeding_limits_fails() {
    ResourceGovernor governor;
    governor.set_budget(make_budget());
    CHECK(governor.consume(ResourceType::CPU, 6));
    CHECK(!governor.consume(ResourceType::CPU, 5));
    CHECK(governor.current_usage().used.at(ResourceType::CPU) == 6);
    return true;
}

static bool test_11_resource_consume_runtime() {
    ResourceGovernor governor;
    governor.set_budget(make_budget());
    CHECK(governor.consume_runtime(5));
    CHECK(!governor.consume_runtime(4));
    CHECK(governor.current_usage().runtime_hours_used == 5);
    return true;
}

static bool test_12_resource_within_budget() {
    ResourceGovernor governor;
    governor.set_budget(make_budget());
    CHECK(governor.consume(ResourceType::CPU, 10));
    CHECK(governor.consume_runtime(8));
    CHECK(governor.consume_experiment());
    CHECK(governor.consume_trial());
    CHECK(governor.within_budget());
    CHECK(governor.consume_trial());
    CHECK(governor.consume_trial());
    CHECK(governor.within_budget());
    CHECK(!governor.consume_trial());
    return true;
}

static bool test_13_recovery_plan() {
    CrashRecoveryManager manager;
    const auto plan = make_plan(1);
    CHECK(manager.plan(plan));
    CHECK(manager.contains(plan.plan_id));
    CHECK(manager.plan_count() == 1);
    return true;
}

static bool test_14_recovery_record_report() {
    CrashRecoveryManager manager;
    const auto plan = make_plan(1);
    CHECK(manager.plan(plan));
    CHECK(manager.record_report(RecoveryReport(make_id(2), plan.plan_id,
                                               RecoveryPhase::COMPLETED, true, "restored", Timestamp(8))));
    CHECK(manager.all_reports().size() == 1);
    return true;
}

static bool test_15_recovery_get_plan() {
    CrashRecoveryManager manager;
    const auto plan = make_plan(1);
    CHECK(manager.plan(plan));
    RecoveryPlan out;
    CHECK(manager.get_plan(plan.plan_id, out));
    CHECK(out.reason == "crash");
    return true;
}

static bool test_16_orchestrator_end_to_end() {
    OperatingWindowOrchestrator orchestrator;
    CHECK(orchestrator.configure_schedule(make_schedule()));
    CHECK(orchestrator.current_mode() == OperatingMode::SCHEDULED);
    CHECK(orchestrator.transition_mode(OperatingMode::ACTIVE, Timestamp(3), "activate"));
    CHECK(orchestrator.store_checkpoint(make_checkpoint(1)));
    CHECK(orchestrator.update_checkpoint_status(make_id(1), CheckpointStatus::VALIDATED));
    CHECK(orchestrator.checkpoint_count() == 1);
    CHECK(orchestrator.plan_recovery(make_plan(2)));
    CHECK(orchestrator.record_recovery_report(RecoveryReport(make_id(3), make_id(2), RecoveryPhase::COMPLETED, true, "ok", Timestamp(9))));
    CHECK(orchestrator.recovery_plan_count() == 1);
    return true;
}

static bool test_17_orchestrator_resource_flow() {
    OperatingWindowOrchestrator orchestrator;
    orchestrator.set_resource_budget(make_budget());
    CHECK(orchestrator.consume_resource(ResourceType::CPU, 10));
    CHECK(orchestrator.within_budget());
    CHECK(!orchestrator.consume_resource(ResourceType::CPU, 1));
    return true;
}

static bool test_18_orchestrator_recovery_flow() {
    OperatingWindowOrchestrator orchestrator;
    const auto plan = make_plan(5);
    CHECK(orchestrator.plan_recovery(plan));
    CHECK(orchestrator.record_recovery_report(RecoveryReport(make_id(6), plan.plan_id,
                                                            RecoveryPhase::FAILED, false, "failed", Timestamp(10))));
    CHECK(orchestrator.recovery_plan_count() == 1);
    return true;
}

static bool test_19_orchestrator_clear() {
    OperatingWindowOrchestrator orchestrator;
    CHECK(orchestrator.configure_schedule(make_schedule()));
    CHECK(orchestrator.store_checkpoint(make_checkpoint(1)));
    orchestrator.set_resource_budget(make_budget());
    CHECK(orchestrator.consume_resource(ResourceType::CPU, 1));
    CHECK(orchestrator.plan_recovery(make_plan(2)));
    orchestrator.clear();
    CHECK(orchestrator.current_mode() == OperatingMode::OFFLINE);
    CHECK(orchestrator.checkpoint_count() == 0);
    CHECK(orchestrator.recovery_plan_count() == 0);
    return true;
}

static bool test_20_enum_sanity() {
    CHECK(static_cast<std::uint8_t>(OperatingMode::OFFLINE) == 0);
    CHECK(static_cast<std::uint8_t>(OperatingMode::EMERGENCY_STOP) == 7);
    CHECK(static_cast<std::uint8_t>(RecoveryPhase::NONE) == 0);
    CHECK(static_cast<std::uint8_t>(RecoveryPhase::ABORTED) == 7);
    CHECK(static_cast<std::uint8_t>(CheckpointStatus::CREATED) == 0);
    CHECK(static_cast<std::uint8_t>(CheckpointStatus::EXPIRED) == 4);
    return true;
}
} // namespace
} // namespace xauusd::sovereign::tests

int main() {
    using namespace xauusd::sovereign::tests;
    const bool results[] = {
        test_1_schedule_configure(), test_2_schedule_transition_to(),
        test_3_schedule_event_count(), test_4_checkpoint_store(),
        test_5_checkpoint_duplicate_rejection(), test_6_checkpoint_update_status(),
        test_7_checkpoint_get(), test_8_resource_set_budget(),
        test_9_resource_consume_within_limits(), test_10_resource_exceeding_limits_fails(),
        test_11_resource_consume_runtime(), test_12_resource_within_budget(),
        test_13_recovery_plan(), test_14_recovery_record_report(),
        test_15_recovery_get_plan(), test_16_orchestrator_end_to_end(),
        test_17_orchestrator_resource_flow(), test_18_orchestrator_recovery_flow(),
        test_19_orchestrator_clear(), test_20_enum_sanity()
    };
    for (bool ok : results) {
        if (!ok) return 1;
    }
    return 0;
}
