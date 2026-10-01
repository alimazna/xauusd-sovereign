#pragma once
#include "EntityId.h"
#include "RecoveryPlan.h"
#include "RecoveryReport.h"
#include <cstddef>
#include <vector>
namespace xauusd::sovereign {
class ICrashRecoveryManager {
public:
    virtual ~ICrashRecoveryManager() = default;
    virtual bool plan(const RecoveryPlan& plan) = 0;
    virtual bool record_report(const RecoveryReport& report) = 0;
    virtual bool contains(const EntityId& plan_id) const = 0;
    virtual bool get_plan(const EntityId& plan_id, RecoveryPlan& out) const = 0;
    virtual std::vector<RecoveryReport> all_reports() const = 0;
    virtual std::size_t plan_count() const = 0;
};
}
