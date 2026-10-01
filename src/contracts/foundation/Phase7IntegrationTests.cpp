#include "ApprovalGate.h"
#include "ChangeInvestigator.h"
#include "PromotionGate.h"
#include "KnownGoodRegistry.h"
#include "RollbackManager.h"
#include "IncidentTracker.h"
#include "GovernanceOrchestrator.h"

#include <array>
#include <cstdint>
#include <vector>

namespace xauusd::sovereign::tests {
namespace {
#define CHECK(expr) do { if (!(expr)) { return false; } } while (false)

static EntityId make_id(std::uint8_t seed) {
    std::array<std::uint8_t, 16> bytes{};
    bytes[0] = seed;
    return EntityId{bytes};
}

static ApprovalRequest make_approval_request(std::uint8_t seed) {
    return ApprovalRequest(make_id(seed), make_id(seed + 10U), make_id(seed + 20U),
                           Version(1), Version(2), "approval", Timestamp(10));
}

static ChangeProposal make_proposal(std::uint8_t seed) {
    return ChangeProposal(make_id(seed), make_id(seed + 10U), ChangeProposalStatus::DRAFTED,
                          "title", "problem", "change", "effect", "risks",
                          Version(1), Version(2), Timestamp(10), Timestamp(11));
}

static PromotionPackage make_promotion(std::uint8_t seed) {
    return PromotionPackage(make_id(seed), make_id(seed + 10U), make_id(seed + 20U),
                             PromotionStatus::NOT_READY, Version(1), Version(2), Version(1),
                             "notes", Timestamp(12));
}

static KnownGoodVersion make_known_good(std::uint8_t seed, std::uint64_t version) {
    return KnownGoodVersion(make_id(seed), Version(version), make_id(seed + 20U),
                            "known good", Timestamp(13), true);
}

static Incident make_incident(std::uint8_t seed, bool resolved = false) {
    return Incident(make_id(seed), IncidentSeverity::HIGH, make_id(seed + 10U), Version(2),
                     "summary", "hypothesis", "prevention", Timestamp(20), Timestamp(21), resolved);
}

static RollbackRequest make_rollback_request(std::uint8_t seed) {
    return RollbackRequest(make_id(seed), Version(2), Version(1), make_id(seed + 30U),
                           "rollback", Timestamp(22));
}

static bool test_1_approval_submit() {
    ApprovalGate gate;
    CHECK(gate.submit(make_approval_request(1)));
    CHECK(gate.pending_count() == 1);
    CHECK(gate.record_count() == 0);
    return true;
}

static bool test_2_approval_duplicate_rejection() {
    ApprovalGate gate;
    const auto request = make_approval_request(1);
    CHECK(gate.submit(request));
    CHECK(!gate.submit(request));
    return true;
}

static bool test_3_approval_decide() {
    ApprovalGate gate;
    const auto request = make_approval_request(1);
    CHECK(gate.submit(request));
    CHECK(gate.decide(ApprovalDecision(make_id(2), request.request_id, ApprovalStatus::APPROVED,
                                       "approved", Timestamp(23))));
    const auto records = gate.all_records();
    CHECK(records.size() == 1);
    CHECK(records.front().decision.status == ApprovalStatus::APPROVED);
    CHECK(!gate.decide(ApprovalDecision(make_id(3), request.request_id, ApprovalStatus::REJECTED,
                                        "duplicate", Timestamp(24))));
    return true;
}

static bool test_4_approval_pending_count_after_decide() {
    ApprovalGate gate;
    const auto a = make_approval_request(1);
    const auto b = make_approval_request(2);
    CHECK(gate.submit(a));
    CHECK(gate.submit(b));
    CHECK(gate.decide(ApprovalDecision(make_id(3), a.request_id, ApprovalStatus::APPROVED,
                                       "ok", Timestamp(23))));
    CHECK(gate.pending_count() == 1);
    return true;
}

static bool test_5_change_submit_get() {
    ChangeInvestigator investigator;
    const auto proposal = make_proposal(1);
    CHECK(investigator.submit(proposal));
    ChangeProposal out;
    CHECK(investigator.get(proposal.proposal_id, out));
    CHECK(out.title == "title");
    CHECK(investigator.size() == 1);
    return true;
}

static bool test_6_change_update_status() {
    ChangeInvestigator investigator;
    const auto proposal = make_proposal(1);
    CHECK(investigator.submit(proposal));
    CHECK(investigator.update_status(proposal.proposal_id, ChangeProposalStatus::EVIDENCE_READY));
    ChangeProposal out;
    CHECK(investigator.get(proposal.proposal_id, out));
    CHECK(out.status == ChangeProposalStatus::EVIDENCE_READY);
    return true;
}

static bool test_7_change_attach_report() {
    ChangeInvestigator investigator;
    const auto proposal = make_proposal(1);
    CHECK(investigator.submit(proposal));
    InvestigationReport report(make_id(40), proposal.proposal_id, {}, "evidence", "risks", "unknowns", Timestamp(30));
    CHECK(investigator.attach_report(report));
    const auto out = investigator.report_for(proposal.proposal_id);
    CHECK(out.report_id == report.report_id);
    CHECK(out.evidence_summary == "evidence");
    return true;
}

static bool test_8_promotion_submit_get() {
    PromotionGate gate;
    const auto package = make_promotion(1);
    CHECK(gate.submit(package));
    PromotionPackage out;
    CHECK(gate.get(package.package_id, out));
    CHECK(out.candidate_version == Version(2));
    return true;
}

static bool test_9_promotion_update_status() {
    PromotionGate gate;
    const auto package = make_promotion(1);
    CHECK(gate.submit(package));
    CHECK(gate.update_status(package.package_id, PromotionStatus::READY_FOR_REVIEW));
    PromotionPackage out;
    CHECK(gate.get(package.package_id, out));
    CHECK(out.status == PromotionStatus::READY_FOR_REVIEW);
    return true;
}

static bool test_10_known_good_register() {
    KnownGoodRegistry registry;
    CHECK(registry.register_version(make_known_good(1, 3)));
    CHECK(registry.contains_version(Version(3)));
    CHECK(registry.size() == 1);
    return true;
}

static bool test_11_known_good_duplicate_rejection() {
    KnownGoodRegistry registry;
    const auto entry = make_known_good(1, 3);
    CHECK(registry.register_version(entry));
    CHECK(!registry.register_version(entry));
    return true;
}

static bool test_12_known_good_latest() {
    KnownGoodRegistry registry;
    CHECK(registry.register_version(make_known_good(1, 3)));
    CHECK(registry.register_version(make_known_good(2, 7)));
    CHECK(registry.latest().version == Version(7));
    return true;
}

static bool test_13_incident_record() {
    IncidentTracker tracker;
    CHECK(tracker.record(make_incident(1)));
    CHECK(tracker.contains(make_id(1)));
    CHECK(tracker.size() == 1);
    return true;
}

static bool test_14_incident_resolve() {
    IncidentTracker tracker;
    CHECK(tracker.record(make_incident(1)));
    CHECK(tracker.resolve(make_id(1)));
    Incident out;
    CHECK(tracker.get(make_id(1), out));
    CHECK(out.resolved);
    return true;
}

static bool test_15_incident_unresolved() {
    IncidentTracker tracker;
    CHECK(tracker.record(make_incident(1)));
    CHECK(tracker.record(make_incident(2, true)));
    const auto unresolved = tracker.unresolved();
    CHECK(unresolved.size() == 1);
    CHECK(unresolved.front().incident_id == make_id(1));
    return true;
}

static bool test_16_rollback_submit() {
    RollbackManager manager;
    const auto request = make_rollback_request(1);
    CHECK(manager.submit(request));
    CHECK(manager.contains(request.request_id));
    CHECK(manager.request_count() == 1);
    return true;
}

static bool test_17_rollback_record_result() {
    RollbackManager manager;
    const auto request = make_rollback_request(1);
    CHECK(manager.submit(request));
    CHECK(manager.record_result(RollbackResult(make_id(2), request, true, "done", Timestamp(23))));
    CHECK(manager.all_results().size() == 1);
    CHECK(!manager.record_result(RollbackResult(make_id(3), make_rollback_request(99), false, "missing", Timestamp(24))));
    return true;
}

static bool test_18_governance_end_to_end() {
    GovernanceOrchestrator orchestrator;
    const auto request = make_approval_request(1);
    CHECK(orchestrator.submit_approval(request));
    CHECK(orchestrator.pending_approval_count() == 1);
    CHECK(orchestrator.decide_approval(ApprovalDecision(make_id(2), request.request_id, ApprovalStatus::APPROVED, "ok", Timestamp(25))));

    const auto proposal = make_proposal(3);
    CHECK(orchestrator.submit_proposal(proposal));
    CHECK(orchestrator.update_proposal_status(proposal.proposal_id, ChangeProposalStatus::UNDER_REVIEW));
    CHECK(orchestrator.attach_report(InvestigationReport(make_id(4), proposal.proposal_id, {}, "e", "r", "u", Timestamp(26))));

    const auto package = make_promotion(5);
    CHECK(orchestrator.submit_promotion(package));
    CHECK(orchestrator.update_promotion_status(package.package_id, PromotionStatus::APPROVED_FOR_PROMOTION));
    CHECK(orchestrator.register_known_good(make_known_good(6, 4)));

    const auto incident = make_incident(7);
    CHECK(orchestrator.record_incident(incident));
    CHECK(orchestrator.resolve_incident(incident.incident_id));

    const auto rollback = make_rollback_request(8);
    CHECK(orchestrator.submit_rollback(rollback));
    CHECK(orchestrator.record_rollback_result(RollbackResult(make_id(9), rollback, true, "done", Timestamp(27))));
    return true;
}

static bool test_19_governance_clear() {
    GovernanceOrchestrator orchestrator;
    const auto request = make_approval_request(1);
    CHECK(orchestrator.submit_approval(request));
    CHECK(orchestrator.pending_approval_count() == 1);
    orchestrator.clear();
    CHECK(orchestrator.pending_approval_count() == 0);
    CHECK(!orchestrator.decide_approval(ApprovalDecision(make_id(2), request.request_id, ApprovalStatus::APPROVED, "gone", Timestamp(28))));
    return true;
}

static bool test_20_enum_sanity() {
    CHECK(static_cast<std::uint8_t>(ApprovalStatus::PENDING) == 0);
    CHECK(static_cast<std::uint8_t>(ApprovalStatus::EXPIRED) == 6);
    CHECK(static_cast<std::uint8_t>(IncidentSeverity::INFO) == 0);
    CHECK(static_cast<std::uint8_t>(IncidentSeverity::CRITICAL) == 4);
    return true;
}
} // namespace
} // namespace xauusd::sovereign::tests

int main() {
    using namespace xauusd::sovereign::tests;
    const bool results[] = {
        test_1_approval_submit(), test_2_approval_duplicate_rejection(),
        test_3_approval_decide(), test_4_approval_pending_count_after_decide(),
        test_5_change_submit_get(), test_6_change_update_status(),
        test_7_change_attach_report(), test_8_promotion_submit_get(),
        test_9_promotion_update_status(), test_10_known_good_register(),
        test_11_known_good_duplicate_rejection(), test_12_known_good_latest(),
        test_13_incident_record(), test_14_incident_resolve(),
        test_15_incident_unresolved(), test_16_rollback_submit(),
        test_17_rollback_record_result(), test_18_governance_end_to_end(),
        test_19_governance_clear(), test_20_enum_sanity()
    };
    for (bool ok : results) {
        if (!ok) return 1;
    }
    return 0;
}
