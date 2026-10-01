#pragma once

#include "KnowledgeContradiction.h"
#include "KnowledgeObject.h"

#include <vector>

namespace xauusd::sovereign {

class ContradictionEngine {
public:
    ContradictionEngine() = default;

    std::vector<KnowledgeContradiction> detect_conflicts(
        const std::vector<KnowledgeObject>& objects) const;

    bool resolve(const EntityId& contradiction_id);

    void clear();

private:
    mutable std::vector<KnowledgeContradiction> contradictions_;
};

} // namespace xauusd::sovereign
