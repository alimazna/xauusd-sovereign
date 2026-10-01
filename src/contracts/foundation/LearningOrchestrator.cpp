#include "LearningOrchestrator.h"

#include <cmath>

namespace xauusd::sovereign {

bool LearningOrchestrator::add_knowledge(const KnowledgeObject& object) {
    return knowledge_.add(object);
}

bool LearningOrchestrator::update_knowledge(const KnowledgeObject& object) {
    return knowledge_.update(object);
}

std::size_t LearningOrchestrator::knowledge_count() const noexcept {
    return knowledge_.size();
}

const KnowledgeStore& LearningOrchestrator::knowledge() const noexcept {
    return knowledge_;
}

bool LearningOrchestrator::transition_knowledge(
    const KnowledgeTransition& t) {
    return lifecycle_.transition(t);
}

std::vector<KnowledgeHistoryEntry> LearningOrchestrator::knowledge_history(
    const KnowledgeId& id) const {
    return lifecycle_.history_for(id);
}

bool LearningOrchestrator::record_failure(
    const FailureMemoryEntry& entry) {
    return failures_.record(entry);
}

bool LearningOrchestrator::add_hypothesis(const RCAHypothesis& h) {
    return failures_.add_hypothesis(h);
}

std::size_t LearningOrchestrator::failure_count() const noexcept {
    return failures_.size();
}

bool LearningOrchestrator::learn_context(
    const ContextLearningRecord& record) {
    return context_.learn(record);
}

std::size_t LearningOrchestrator::context_count() const noexcept {
    return context_.size();
}

DecayEvaluation LearningOrchestrator::evaluate_decay(
    const KnowledgeObject& object,
    const DecayPolicy& policy,
    Timestamp now) const {
    const std::int64_t age =
        (now.value() >= object.last_supported_at.value())
            ? (now.value() - object.last_supported_at.value())
            : 0;

    const bool is_expired =
        policy.max_age_microseconds > 0 &&
        age >= policy.max_age_microseconds;

    double current_confidence = object.confidence;
    if (policy.half_life_microseconds > 0 && age > 0) {
        const double elapsed_half_lives =
            static_cast<double>(age) /
            static_cast<double>(policy.half_life_microseconds);
        current_confidence *= std::pow(0.5, elapsed_half_lives);
    }

    const bool below_revalidation_threshold =
        current_confidence < policy.revalidation_threshold;
    const bool below_minimum_confidence =
        current_confidence < policy.minimum_confidence;
    const bool should_revalidate =
        is_expired ||
        below_revalidation_threshold ||
        below_minimum_confidence;

    return DecayEvaluation{
        object.knowledge_id,
        should_revalidate,
        is_expired,
        current_confidence,
        now};
}

void LearningOrchestrator::clear() {
    knowledge_.clear();
    lifecycle_.clear();
    contradictions_.clear();
    failures_.clear();
    context_.clear();
}

} // namespace xauusd::sovereign
