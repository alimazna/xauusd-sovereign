#include "OperatingWindowOrchestrator.h"
namespace xauusd::sovereign {
bool OperatingWindowOrchestrator::configure_schedule(const OperatingSchedule& schedule) { return schedule_.configure(schedule); }
OperatingMode OperatingWindowOrchestrator::current_mode() const { return schedule_.current_mode(); }
bool OperatingWindowOrchestrator::transition_mode(OperatingMode new_mode, Timestamp at, const std::string& reason) { return schedule_.transition_to(new_mode, at, reason); }
bool OperatingWindowOrchestrator::store_checkpoint(const Checkpoint& checkpoint) { return checkpoints_.store(checkpoint); }
bool OperatingWindowOrchestrator::update_checkpoint_status(const EntityId& checkpoint_id, CheckpointStatus status) { return checkpoints_.update_status(checkpoint_id, status); }
std::size_t OperatingWindowOrchestrator::checkpoint_count() const { return checkpoints_.size(); }
void OperatingWindowOrchestrator::set_resource_budget(const ResourceBudget& budget) { resources_.set_budget(budget); }
bool OperatingWindowOrchestrator::consume_resource(ResourceType type, std::uint64_t amount) { return resources_.consume(type, amount); }
bool OperatingWindowOrchestrator::within_budget() const { return resources_.within_budget(); }
bool OperatingWindowOrchestrator::plan_recovery(const RecoveryPlan& plan) { return recovery_.plan(plan); }
bool OperatingWindowOrchestrator::record_recovery_report(const RecoveryReport& report) { return recovery_.record_report(report); }
std::size_t OperatingWindowOrchestrator::recovery_plan_count() const { return recovery_.plan_count(); }
void OperatingWindowOrchestrator::clear() {
    schedule_.clear();
    checkpoints_.clear();
    resources_.clear();
    recovery_.clear();
}
} // namespace xauusd::sovereign
