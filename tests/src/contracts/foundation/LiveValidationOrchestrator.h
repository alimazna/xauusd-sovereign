#pragma once

#include "CanaryController.h"
#include "KillSwitch.h"
#include "LiveController.h"
#include "MT5Bridge.h"
#include "SafetyController.h"

namespace xauusd::sovereign {

class LiveValidationOrchestrator {
public:
    LiveValidationOrchestrator() = default;

    // Live Controller
    bool enable_live(const LiveConfig& config);
    bool disable_live();
    bool live_enabled() const;
    LiveModeStatus live_status() const;

    // Canary
    bool configure_canary(const CanaryConfig& config);
    CanaryStage canary_stage() const;
    bool submit_scale_up(const ScaleUpRequest& request);
    bool decide_scale_up(const ScaleUpDecision& decision);

    // Safety
    bool engage_kill_switch(const KillSwitch& kill_switch);
    bool release_kill_switch();
    bool kill_switch_engaged() const;
    bool record_safety_check(const SafetyCheck& check);
    bool all_safety_checks_passed() const;
    bool record_live_incident(const LiveIncident& incident);

    // Reset
    void clear();

private:
    LiveController      live_;
    CanaryController    canary_;
    SafetyController    safety_;
    MT5Bridge           bridge_;
};

} // namespace xauusd::sovereign
