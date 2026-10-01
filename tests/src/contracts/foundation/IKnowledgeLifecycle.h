#pragma once

#include "KnowledgeContradiction.h"
#include "KnowledgeHistoryEntry.h"
#include "KnowledgeTransition.h"

#include <vector>

namespace xauusd::sovereign {

class IKnowledgeLifecycle {
public:
    virtual ~IKnowledgeLifecycle() = default;

    virtual bool transition(const KnowledgeTransition& t) = 0;
    virtual std::vector<KnowledgeHistoryEntry> history_for(const KnowledgeId& id) const = 0;
    virtual bool register_contradiction(const KnowledgeContradiction& c) = 0;
    virtual std::size_t transition_count() const = 0;
};

} // namespace xauusd::sovereign
