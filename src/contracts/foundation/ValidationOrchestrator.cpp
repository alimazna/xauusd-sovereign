#include "ValidationOrchestrator.h"

namespace xauusd::sovereign {

bool ValidationOrchestrator::register_evidence_layer(const EvidenceLayer& layer) {
    return evidence_.register_layer(layer);
}

bool ValidationOrchestrator::request_holdout(const HoldoutQuery& q) {
    return evidence_.request_holdout(q);
}

std::size_t ValidationOrchestrator::holdout_budget_remaining() const {
    return evidence_.holdout_budget_remaining();
}

bool ValidationOrchestrator::register_protocol(const ValidationProtocol& protocol) {
    return validation_.register_protocol(protocol);
}

bool ValidationOrchestrator::record_validation_run(const ValidationRun& run) {
    return validation_.record_run(run);
}

bool ValidationOrchestrator::record_validation_result(const ValidationResult& result) {
    return validation_.record_result(result);
}

bool ValidationOrchestrator::candidate_passed(const EntityId& run_id) const {
    return validation_.all_passed(run_id);
}

bool ValidationOrchestrator::register_evaluator(const EvaluatorIdentity& identity) {
    return evaluators_.register_evaluator(identity);
}

bool ValidationOrchestrator::freeze_evaluator(const EntityId& evaluator_id) {
    return evaluators_.freeze(evaluator_id);
}

ContaminationState ValidationOrchestrator::contamination_state(
    const EntityId& candidate_id) const {
    ContaminationState state = ContaminationState::CLEAN;
    contamination_.get(candidate_id, state);
    return state;
}

bool ValidationOrchestrator::mark_contaminated(const EntityId& candidate_id,
                                               ContaminationState state) {
    return contamination_.mark(candidate_id, state);
}

void ValidationOrchestrator::clear() {
    evidence_.clear();
    validation_.clear();
    evaluators_.clear();
    contamination_.clear();
}

} // namespace xauusd::sovereign
