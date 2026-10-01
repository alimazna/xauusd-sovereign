#pragma once

#include "KnowledgeId.h"
#include "KnowledgeStatus.h"
#include "Timestamp.h"
#include "Version.h"

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace xauusd::sovereign {

struct KnowledgeObject {
    KnowledgeId             knowledge_id;
    std::string             observation;
    std::string             context;
    std::string             conclusion;
    std::string             validity_scope;
    KnowledgeStatus         status;
    double                  confidence;
    Timestamp               first_observed_at;
    Timestamp               last_supported_at;
    Timestamp               revalidation_due_at;
    std::vector<EntityId>   evidence_refs;
    std::vector<EntityId>   hypothesis_refs;
    std::vector<EntityId>   experiment_refs;
    std::vector<EntityId>   related_failures;
    std::vector<EntityId>   contradictory_refs;
    std::vector<EntityId>   lineage_refs;
    std::vector<EntityId>   human_decision_refs;

    KnowledgeObject() = default;

    KnowledgeObject(
        KnowledgeId knowledge_id,
        std::string observation,
        std::string context,
        std::string conclusion,
        std::string validity_scope,
        KnowledgeStatus status,
        double confidence,
        Timestamp first_observed_at,
        Timestamp last_supported_at,
        Timestamp revalidation_due_at,
        std::vector<EntityId> evidence_refs,
        std::vector<EntityId> hypothesis_refs,
        std::vector<EntityId> experiment_refs,
        std::vector<EntityId> related_failures,
        std::vector<EntityId> contradictory_refs,
        std::vector<EntityId> lineage_refs,
        std::vector<EntityId> human_decision_refs)
        : knowledge_id(knowledge_id),
          observation(std::move(observation)),
          context(std::move(context)),
          conclusion(std::move(conclusion)),
          validity_scope(std::move(validity_scope)),
          status(status),
          confidence(confidence),
          first_observed_at(first_observed_at),
          last_supported_at(last_supported_at),
          revalidation_due_at(revalidation_due_at),
          evidence_refs(std::move(evidence_refs)),
          hypothesis_refs(std::move(hypothesis_refs)),
          experiment_refs(std::move(experiment_refs)),
          related_failures(std::move(related_failures)),
          contradictory_refs(std::move(contradictory_refs)),
          lineage_refs(std::move(lineage_refs)),
          human_decision_refs(std::move(human_decision_refs)) {}
};

} // namespace xauusd::sovereign
