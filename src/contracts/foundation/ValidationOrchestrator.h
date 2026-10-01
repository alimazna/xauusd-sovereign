#pragma once

#include "ContaminationTracker.h"
#include "EvaluatorFirewall.h"
#include "EvidenceFirewall.h"
#include "ValidationFirewall.h"

namespace xauusd::sovereign {

class ValidationOrchestrator {
public:
    ValidationOrchestrator() = default;

    // Evidence
    bool register_evidence_layer(const EvidenceLayer& layer);
    bool request_holdout(const HoldoutQuery& q);
    std::size_t holdout_budget_remaining() const;

    // Validation
    bool register_protocol(const ValidationProtocol& protocol);
    bool record_validation_run(const ValidationRun& run);
    bool record_validation_result(const ValidationResult& result);
    bool candidate_passed(const EntityId& run_id) const;

    // Evaluator
    bool register_evaluator(const EvaluatorIdentity& identity);
    bool freeze_evaluator(const EntityId& evaluator_id);

    // Contamination
    ContaminationState contamination_state(const EntityId& candidate_id) const;
    bool mark_contaminated(const EntityId& candidate_id, ContaminationState state);

    // Reset
    void clear();

private:
    EvidenceFirewall        evidence_;
    ValidationFirewall      validation_;
    EvaluatorFirewall       evaluators_;
    ContaminationTracker    contamination_;
};

} // namespace xauusd::sovereign
