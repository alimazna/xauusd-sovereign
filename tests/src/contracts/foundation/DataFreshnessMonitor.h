#pragma once
#include "CapabilityId.h"
#include "Timestamp.h"
#include "FreshnessState.h"
#include <cstdint>
#include <map>
namespace xauusd::sovereign {
class DataFreshnessMonitor {
public:
    DataFreshnessMonitor() = default;
    void observe(CapabilityId capability_id, Timestamp observed_at);
    FreshnessState evaluate(CapabilityId capability_id, Timestamp now,
                            std::int64_t fresh_threshold_us,
                            std::int64_t aging_threshold_us) const;
    void clear() noexcept;
private:
    std::map<CapabilityId, Timestamp> latest_;
};
}
