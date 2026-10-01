#pragma once

#include "IKnowledgeLifecycle.h"

#include <vector>

namespace xauusd::sovereign {

class KnowledgeLifecycle final : public IKnowledgeLifecycle {
public:
    KnowledgeLifecycle() = default;

    bool transition(const KnowledgeTransition& t) override;
    std::vector<KnowledgeHistoryEntry> history_for(const KnowledgeId& id) const override;
    bool register_contradiction(const KnowledgeContradiction& c) override;
    std::size_t transition_count() const override;

    void clear();

private:
    std::vector<KnowledgeHistoryEntry> history_;
    std::vector<KnowledgeContradiction> contradictions_;
};

} // namespace xauusd::sovereign
