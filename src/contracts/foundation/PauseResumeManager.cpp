#include "PauseResumeManager.h"
namespace xauusd::sovereign {
bool PauseResumeManager::pause(CapabilityId capability_id, Timestamp at) { if (paused_.find(capability_id) != paused_.end()) return false; paused_.emplace(capability_id, PauseRecord{at}); return true; }
bool PauseResumeManager::resume(CapabilityId capability_id, Timestamp at) { (void)at; return paused_.erase(capability_id) != 0; }
bool PauseResumeManager::is_paused(CapabilityId capability_id) const { return paused_.find(capability_id) != paused_.end(); }
std::vector<CapabilityId> PauseResumeManager::paused_capabilities() const { std::vector<CapabilityId> result; result.reserve(paused_.size()); for (const auto& [id, record] : paused_) { (void)record; result.push_back(id); } return result; }
}
