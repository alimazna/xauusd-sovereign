#include "CanaryController.h"
#include "LiveController.h"
#include "LiveValidationOrchestrator.h"
#include "SafetyController.h"

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace xauusd::sovereign::tests {
namespace {
#define CHECK(expr) do { if (!(expr)) { return false; } } while (false)

static EntityId make_id(std::uint8_t seed) {
    std::array<std::uint8_t, 16> bytes{};
    bytes[0] = seed;
    return EntityId{bytes};
}

static LiveConfig make_live_config() {
    return LiveConfig(10, 100, 500, 3, "XAUUSD", "USD", true);
}

static LiveDecision make_decision(std::uint8_t seed, bool accepted) {
    return LiveDecision(
        make_id(seed), make_id(50), make_id(100 + seed), accepted,
        accepted ? "accepted" : "rejected", Timestamp(seed));
}

static CanaryConfig make_canary_config(bool human = true) {
    return CanaryConfig(CanaryStage::STAGE_1_MINIMAL, 100, 200, human);
}

static ScaleUpRequest make_scale_request(std::uint8_t seed) {
    return ScaleUpRequest(
        make_id(seed), CanaryStage::STAGE_1_MINIMAL,
        CanaryStage::STAGE_2_SMALL, "promotion", Timestamp(seed));
}

static SafetyCheck make_check(std::uint8_t seed, bool passed) {
    return SafetyCheck(
        make_id(seed), passed ? "pass" : "fail", passed,
        passed ? "" : "failure", Timestamp(seed), std::vector<EntityId>{});
}

static LiveIncident make_incident(std::uint8_t seed) {
    return LiveIncident(
        make_id(seed), IncidentSeverity::HIGH,
        "incident", "contain", Timestamp(seed), false);
}

static KillSwitch make_kill_switch(std::uint8_t seed) {
    return KillSwitch(
        make_id(seed), true, "stop", Timestamp(seed), "human");
}

static bool test_1_live_enable() {
    LiveController controller;
    CHECK(controller.enable(make_live_config()));
    CHECK(controller.is_enabled());
    return true;
}

static bool test_2_live_status() {
    LiveController controller;
    CHECK(controller.current_status() == LiveModeStatus::DISABLED);
    CHECK(controller.enable(make_live_config()));
    CHECK(controller.current_status() == LiveModeStatus::MANUAL_ONLY);
    return true;
}

static bool test_3_live_record_accepted() {
    LiveController controller;
    CHECK(controller.enable(make_live_config()));
    CHECK(controller.record_decision(make_decision(1, true)));
    CHECK(controller.current_session().trades_executed == 1);
    return true;
}

static bool test_4_live_record_rejected() {
    LiveController controller;
    CHECK(controller.enable(make_live_config()));
    CHECK(controller.record_decision(make_decision(2, false)));
    CHECK(controller.current_session().trades_rejected == 1);
    return true;
}

static bool test_5_live_decision_count() {
    LiveController controller;
    CHECK(controller.enable(make_live_config()));
    CHECK(controller.record_decision(make_decision(3, true)));
    CHECK(controller.record_decision(make_decision(4, false)));
    CHECK(controller.decision_count() == 2);
    CHECK(controller.all_decisions().size() == 2);
    return true;
}

static bool test_6_live_disable() {
    LiveController controller;
    CHECK(controller.enable(make_live_config()));
    CHECK(controller.disable());
    CHECK(!controller.is_enabled());
    CHECK(controller.current_status() == LiveModeStatus::DISABLED);
    return true;
}

static bool test_7_live_record_when_disabled_fails() {
    LiveController controller;
    CHECK(!controller.record_decision(make_decision(5, true)));
    CHECK(controller.decision_count() == 0);
    return true;
}

static bool test_8_live_clear() {
    LiveController controller;
    CHECK(controller.enable(make_live_config()));
    CHECK(controller.record_decision(make_decision(6, true)));
    controller.clear();
    CHECK(!controller.is_enabled());
    CHECK(controller.current_status() == LiveModeStatus::DISABLED);
    CHECK(controller.decision_count() == 0);
    return true;
}

static bool test_9_canary_configure() {
    CanaryController controller;
    CHECK(controller.configure(make_canary_config()));
    CHECK(controller.current_stage() == CanaryStage::STAGE_1_MINIMAL);
    return true;
}

static bool test_10_canary_submit_scale_up() {
    CanaryController controller;
    CHECK(controller.configure(make_canary_config()));
    CHECK(controller.submit_scale_up(make_scale_request(10)));
    return true;
}

static bool test_11_canary_decide_approved_promotes() {
    CanaryController controller;
    CHECK(controller.configure(make_canary_config(true)));
    const auto request = make_scale_request(11);
    CHECK(controller.submit_scale_up(request));
    const auto decision = ScaleUpDecision(
        make_id(12), request, true, "approved", Timestamp(12));
    CHECK(controller.decide_scale_up(decision));
    CHECK(controller.current_stage() == CanaryStage::STAGE_2_SMALL);
    return true;
}

static bool test_12_canary_decisions() {
    CanaryController controller;
    CHECK(controller.configure(make_canary_config()));
    const auto request = make_scale_request(13);
    const auto decision = ScaleUpDecision(
        make_id(14), request, false, "rejected", Timestamp(14));
    CHECK(!controller.decide_scale_up(decision));
    CHECK(controller.all_decisions().empty());
    CHECK(controller.current_stage() == CanaryStage::STAGE_1_MINIMAL);
    return true;
}

static bool test_13_safety_kill_switch() {
    SafetyController controller;
    CHECK(!controller.is_kill_switch_engaged());
    CHECK(controller.engage_kill_switch(make_kill_switch(20)));
    CHECK(controller.is_kill_switch_engaged());
    CHECK(controller.release_kill_switch());
    CHECK(!controller.is_kill_switch_engaged());
    return true;
}

static bool test_14_safety_checks_pass() {
    SafetyController controller;
    CHECK(!controller.all_checks_passed());
    CHECK(controller.record_check(make_check(21, true)));
    CHECK(controller.record_check(make_check(22, true)));
    CHECK(controller.all_checks_passed());
    return true;
}

static bool test_15_safety_failed_check() {
    SafetyController controller;
    CHECK(controller.record_check(make_check(23, true)));
    CHECK(controller.record_check(make_check(24, false)));
    CHECK(!controller.all_checks_passed());
    return true;
}

static bool test_16_safety_incident() {
    SafetyController controller;
    CHECK(controller.record_incident(make_incident(25)));
    CHECK(controller.all_incidents().size() == 1);
    CHECK(!controller.all_incidents().front().resolved);
    return true;
}

static bool test_17_safety_clear() {
    SafetyController controller;
    CHECK(controller.engage_kill_switch(make_kill_switch(26)));
    CHECK(controller.record_check(make_check(27, true)));
    CHECK(controller.record_incident(make_incident(28)));
    controller.clear();
    CHECK(!controller.is_kill_switch_engaged());
    CHECK(!controller.all_checks_passed());
    CHECK(controller.all_incidents().empty());
    return true;
}

static bool test_18_orchestrator_live_and_canary() {
    LiveValidationOrchestrator orchestrator;
    CHECK(orchestrator.enable_live(make_live_config()));
    CHECK(orchestrator.live_enabled());
    CHECK(orchestrator.live_status() == LiveModeStatus::MANUAL_ONLY);
    CHECK(orchestrator.configure_canary(make_canary_config(true)));
    const auto request = make_scale_request(29);
    CHECK(orchestrator.submit_scale_up(request));
    CHECK(orchestrator.decide_scale_up(
        ScaleUpDecision(make_id(30), request, true, "approved", Timestamp(30))));
    CHECK(orchestrator.canary_stage() == CanaryStage::STAGE_2_SMALL);
    return true;
}

static bool test_19_orchestrator_safety() {
    LiveValidationOrchestrator orchestrator;
    CHECK(orchestrator.engage_kill_switch(make_kill_switch(31)));
    CHECK(orchestrator.kill_switch_engaged());
    CHECK(orchestrator.record_safety_check(make_check(32, true)));
    CHECK(orchestrator.all_safety_checks_passed());
    CHECK(orchestrator.record_live_incident(make_incident(33)));
    CHECK(orchestrator.release_kill_switch());
    CHECK(!orchestrator.kill_switch_engaged());
    return true;
}

static bool test_20_enum_sanity() {
    CHECK(static_cast<std::uint8_t>(LiveModeStatus::DISABLED) == 0);
    CHECK(static_cast<std::uint8_t>(LiveModeStatus::HALTED) == 6);
    CHECK(static_cast<std::uint8_t>(CanaryStage::STAGE_0_OFF) == 0);
    CHECK(static_cast<std::uint8_t>(CanaryStage::STAGE_4_FULL) == 4);
    return true;
}

} // namespace
} // namespace xauusd::sovereign::tests

int main() {
    using namespace xauusd::sovereign::tests;
    const bool results[] = {
        test_1_live_enable(), test_2_live_status(),
        test_3_live_record_accepted(), test_4_live_record_rejected(),
        test_5_live_decision_count(), test_6_live_disable(),
        test_7_live_record_when_disabled_fails(), test_8_live_clear(),
        test_9_canary_configure(), test_10_canary_submit_scale_up(),
        test_11_canary_decide_approved_promotes(), test_12_canary_decisions(),
        test_13_safety_kill_switch(), test_14_safety_checks_pass(),
        test_15_safety_failed_check(), test_16_safety_incident(),
        test_17_safety_clear(), test_18_orchestrator_live_and_canary(),
        test_19_orchestrator_safety(), test_20_enum_sanity()
    };
    for (bool ok : results) {
        if (!ok) return 1;
    }
    return 0;
}
