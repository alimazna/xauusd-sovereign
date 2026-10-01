#pragma once

#include "EntityId.h"
#include "KillSwitch.h"
#include "LiveIncident.h"
#include "SafetyCheck.h"

#include <cstddef>
#include <vector>

namespace xauusd::sovereign {

class ISafetyController {
public:
    virtual ~ISafetyController() = default;

    virtual bool engage_kill_switch(const KillSwitch& kill_switch) = 0;
    virtual bool release_kill_switch() = 0;
    virtual bool is_kill_switch_engaged() const = 0;

    virtual bool record_check(const SafetyCheck& check) = 0;
    virtual bool all_checks_passed() const = 0;

    virtual bool record_incident(const LiveIncident& incident) = 0;
    virtual std::vector<LiveIncident> all_incidents() const = 0;
};

} // namespace xauusd::sovereign
