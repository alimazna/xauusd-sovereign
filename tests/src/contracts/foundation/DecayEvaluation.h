#pragma once

#include "KnowledgeId.h"
#include "Timestamp.h"

namespace xauusd::sovereign {

struct DecayEvaluation {
    KnowledgeId      knowledge_id;
    bool             should_revalidate;
    bool             is_expired;
    double           current_confidence;
    Timestamp        evaluated_at;

    DecayEvaluation() = default;

    DecayEvaluation(
        KnowledgeId knowledge_id,
        bool should_revalidate,
        bool is_expired,
        double current_confidence,
        Timestamp evaluated_at)
        : knowledge_id(knowledge_id),
          should_revalidate(should_revalidate),
          is_expired(is_expired),
          current_confidence(current_confidence),
          evaluated_at(evaluated_at) {}
};

} // namespace xauusd::sovereign
