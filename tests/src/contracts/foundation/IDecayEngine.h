#pragma once

#include "DecayEvaluation.h"
#include "DecayPolicy.h"
#include "KnowledgeObject.h"

namespace xauusd::sovereign {

class IDecayEngine {
public:
    virtual ~IDecayEngine() = default;

    virtual DecayEvaluation evaluate(
        const KnowledgeObject& object,
        const DecayPolicy& policy,
        Timestamp now) const = 0;

    virtual bool should_revalidate(
        const KnowledgeObject& object,
        const DecayPolicy& policy,
        Timestamp now) const = 0;
};

} // namespace xauusd::sovereign
