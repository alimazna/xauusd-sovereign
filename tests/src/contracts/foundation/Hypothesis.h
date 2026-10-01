#pragma once

#include "EntityId.h"
#include "KnowledgeId.h"
#include "HypothesisClass.h"
#include "HypothesisStatus.h"
#include "Timestamp.h"
#include "Version.h"

#include <cstdint>
#include <string>
#include <utility>

namespace xauusd::sovereign {

class Hypothesis {
public:
    EntityId        hypothesis_id;
    KnowledgeId     parent_knowledge_id;
    EntityId        failure_id;
    HypothesisClass hypothesis_class;
    HypothesisStatus status;
    std::string     claim;
    std::string     context_scope;
    std::string     expected_effect;
    std::string     success_criterion;
    std::string     failure_criterion;
    std::string     no_effect_criterion;
    double          confidence;
    Timestamp       created_at;
    Version         creator_version;

    Hypothesis() = default;

    Hypothesis(
        EntityId hypothesis_id,
        KnowledgeId parent_knowledge_id,
        EntityId failure_id,
        HypothesisClass hypothesis_class,
        HypothesisStatus status,
        std::string claim,
        std::string context_scope,
        std::string expected_effect,
        std::string success_criterion,
        std::string failure_criterion,
        std::string no_effect_criterion,
        double confidence,
        Timestamp created_at,
        Version creator_version)
        : hypothesis_id(hypothesis_id),
          parent_knowledge_id(parent_knowledge_id),
          failure_id(failure_id),
          hypothesis_class(hypothesis_class),
          status(status),
          claim(std::move(claim)),
          context_scope(std::move(context_scope)),
          expected_effect(std::move(expected_effect)),
          success_criterion(std::move(success_criterion)),
          failure_criterion(std::move(failure_criterion)),
          no_effect_criterion(std::move(no_effect_criterion)),
          confidence(confidence),
          created_at(created_at),
          creator_version(creator_version) {}
};

} // namespace xauusd::sovereign
