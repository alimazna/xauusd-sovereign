#include "CrashRecoveryManager.h"
namespace xauusd::sovereign {
bool CrashRecoveryManager::plan(const RecoveryPlan& recovery_plan) {
    const auto key = recovery_plan.plan_id.bytes();
    if (index_.find(key) != index_.end()) {
        return false;
    }
    plans_.push_back(recovery_plan);
    index_.emplace(key, plans_.size() - 1U);
    return true;
}

bool CrashRecoveryManager::record_report(const RecoveryReport& report) {
    const auto key = report.plan_id.bytes();
    if (index_.find(key) == index_.end()) {
        return false;
    }
    reports_.push_back(report);
    return true;
}

bool CrashRecoveryManager::contains(const EntityId& plan_id) const {
    return index_.find(plan_id.bytes()) != index_.end();
}

bool CrashRecoveryManager::get_plan(const EntityId& plan_id, RecoveryPlan& out) const {
    const auto it = index_.find(plan_id.bytes());
    if (it == index_.end()) {
        return false;
    }
    out = plans_[it->second];
    return true;
}

std::vector<RecoveryReport> CrashRecoveryManager::all_reports() const { return reports_; }
std::size_t CrashRecoveryManager::plan_count() const { return plans_.size(); }

void CrashRecoveryManager::clear() {
    plans_.clear();
    index_.clear();
    reports_.clear();
}
} // namespace xauusd::sovereign
