#include "LiveValidationOrchestrator.h"

namespace xauusd::sovereign {

bool LiveValidationOrchestrator::enable_live(const LiveConfig& config) {
    return live_.enable(config);
}

bool LiveValidationOrchestrator::disable_live() {
    return live_.disable();
}

bool LiveValidationOrchestrator::live_enabled() const {
    return live_.is_enabled();
}

LiveModeStatus LiveValidationOrchestrator::live_status() const {
    return live_.current_status();
}

bool LiveValidationOrchestrator::configure_canary(const CanaryConfig& config) {
    return canary_.configure(config);
}

CanaryStage LiveValidationOrchestrator::canary_stage() const {
    return canary_.current_stage();
}

bool LiveValidationOrchestrator::submit_scale_up(const ScaleUpRequest& request) {
    return canary_.submit_scale_up(request);
}

bool LiveValidationOrchestrator::decide_scale_up(const ScaleUpDecision& decision) {
    return canary_.decide_scale_up(decision);
}

bool LiveValidationOrchestrator::engage_kill_switch(const KillSwitch& kill_switch) {
    return safety_.engage_kill_switch(kill_switch);
}

bool LiveValidationOrchestrator::release_kill_switch() {
    return safety_.release_kill_switch();
}

bool LiveValidationOrchestrator::kill_switch_engaged() const {
    return safety_.is_kill_switch_engaged();
}

bool LiveValidationOrchestrator::record_safety_check(const SafetyCheck& check) {
    return safety_.record_check(check);
}

bool LiveValidationOrchestrator::all_safety_checks_passed() const {
    return safety_.all_checks_passed();
}

bool LiveValidationOrchestrator::record_live_incident(const LiveIncident& incident) {
    return safety_.record_incident(incident);
}

void LiveValidationOrchestrator::clear() {
    live_.clear();
    canary_.clear();
    safety_.clear();
    bridge_.disconnect();
}

} // namespace xauusd::sovereign
