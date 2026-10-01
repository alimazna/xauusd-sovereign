#pragma once
#include "Version.h"
#include "CapabilityDescriptor.h"
namespace xauusd::sovereign {
struct CriticalityPolicy {
    Version policy_version{};
    CriticalityLevel level{CriticalityLevel::OPTIONAL};
    bool requires_human_intervention{false};
    bool allows_auto_recovery{true};
    bool allows_degraded_operation{true};
    CriticalityPolicy() = default;
    CriticalityPolicy(const Version& version, CriticalityLevel criticality, bool human,
                      bool auto_recovery, bool degraded)
        : policy_version(version), level(criticality), requires_human_intervention(human),
          allows_auto_recovery(auto_recovery), allows_degraded_operation(degraded) {}
};
}
