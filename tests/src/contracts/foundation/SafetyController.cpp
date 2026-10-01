#include "SafetyController.h"

namespace xauusd::sovereign {

bool SafetyController::engage_kill_switch(const KillSwitch& kill_switch) {
    kill_switch_ = kill_switch;
    return true;
}

bool SafetyController::release_kill_switch() {
    kill_switch_.engaged = false;
    return true;
}

bool SafetyController::is_kill_switch_engaged() const {
    return kill_switch_.engaged;
}

bool SafetyController::record_check(const SafetyCheck& check) {
    checks_.push_back(check);
    return true;
}

bool SafetyController::all_checks_passed() const {
    if (checks_.empty()) {
        return false;
    }
    for (const auto& check : checks_) {
        if (!check.passed) {
            return false;
        }
    }
    return true;
}

bool SafetyController::record_incident(const LiveIncident& incident) {
    incidents_.push_back(incident);
    return true;
}

std::vector<LiveIncident> SafetyController::all_incidents() const {
    return incidents_;
}

void SafetyController::clear() {
    kill_switch_ = KillSwitch{};
    checks_.clear();
    incidents_.clear();
}

} // namespace xauusd::sovereign
