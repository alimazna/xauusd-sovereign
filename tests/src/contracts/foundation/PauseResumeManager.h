#pragma once
#include "CapabilityId.h"
#include "Timestamp.h"
#include <map>
#include <vector>
namespace xauusd::sovereign {
class PauseResumeManager {
public:
    PauseResumeManager() = default;
    bool pause(CapabilityId capability_id, Timestamp at);
    bool resume(CapabilityId capability_id, Timestamp at);
    bool is_paused(CapabilityId capability_id) const;
    std::vector<CapabilityId> paused_capabilities() const;
private:
    struct PauseRecord { Timestamp paused_at; };
    std::map<CapabilityId, PauseRecord> paused_;
};
}
