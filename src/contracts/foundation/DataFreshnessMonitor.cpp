#include "DataFreshnessMonitor.h"
namespace xauusd::sovereign {
void DataFreshnessMonitor::observe(CapabilityId capability_id, Timestamp observed_at) { latest_[capability_id] = observed_at; }
FreshnessState DataFreshnessMonitor::evaluate(CapabilityId capability_id, Timestamp now,
                                              std::int64_t fresh_threshold_us,
                                              std::int64_t aging_threshold_us) const {
    const auto it = latest_.find(capability_id);
    if (it == latest_.end()) return FreshnessState::MISSING;
    const std::int64_t age = now.value() - it->second.value();
    if (age <= fresh_threshold_us) return FreshnessState::FRESH;
    if (age <= aging_threshold_us) return FreshnessState::AGING;
    return FreshnessState::STALE;
}
void DataFreshnessMonitor::clear() noexcept { latest_.clear(); }
}
