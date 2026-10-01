#pragma once

#include "ISafetyController.h"

namespace xauusd::sovereign {

class SafetyController final : public ISafetyController {
public:
    SafetyController() = default;

    bool engage_kill_switch(const KillSwitch& kill_switch) override;
    bool release_kill_switch() override;
    bool is_kill_switch_engaged() const override;

    bool record_check(const SafetyCheck& check) override;
    bool all_checks_passed() const override;

    bool record_incident(const LiveIncident& incident) override;
    std::vector<LiveIncident> all_incidents() const override;

    void clear();

private:
    KillSwitch                 kill_switch_{};
    std::vector<SafetyCheck>   checks_;
    std::vector<LiveIncident>  incidents_;
};

} // namespace xauusd::sovereign
