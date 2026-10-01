#pragma once

#include "ContextLearningEngine.h"
#include "ContradictionEngine.h"
#include "DecayEvaluation.h"
#include "DecayPolicy.h"
#include "FailureMemory.h"
#include "KnowledgeLifecycle.h"
#include "KnowledgeStore.h"

namespace xauusd::sovereign {

class LearningOrchestrator {
public:
    LearningOrchestrator() = default;

    bool add_knowledge(const KnowledgeObject& object);
    bool update_knowledge(const KnowledgeObject& object);
    std::size_t knowledge_count() const noexcept;
    const KnowledgeStore& knowledge() const noexcept;

    bool transition_knowledge(const KnowledgeTransition& t);
    std::vector<KnowledgeHistoryEntry> knowledge_history(const KnowledgeId& id) const;

    bool record_failure(const FailureMemoryEntry& entry);
    bool add_hypothesis(const RCAHypothesis& h);
    std::size_t failure_count() const noexcept;

    bool learn_context(const ContextLearningRecord& record);
    std::size_t context_count() const noexcept;

    DecayEvaluation evaluate_decay(
        const KnowledgeObject& object,
        const DecayPolicy& policy,
        Timestamp now) const;

    void clear();

private:
    KnowledgeStore          knowledge_;
    KnowledgeLifecycle      lifecycle_;
    ContradictionEngine     contradictions_;
    FailureMemory           failures_;
    ContextLearningEngine   context_;
};

} // namespace xauusd::sovereign
