#pragma once
#include "EntityId.h"
#include "Timestamp.h"
#include "ValidationResult.h"
#include <string>
#include <utility>
#include <vector>
namespace xauusd::sovereign {
struct InvestigationReport {
    EntityId                      report_id;
    EntityId                      proposal_id;
    std::vector<ValidationResult> validation_results;
    std::string                   evidence_summary;
    std::string                   known_risks;
    std::string                   unknowns;
    Timestamp                     generated_at;
    InvestigationReport() = default;
    InvestigationReport(EntityId report_id, EntityId proposal_id,
                        std::vector<ValidationResult> validation_results,
                        std::string evidence_summary, std::string known_risks,
                        std::string unknowns, Timestamp generated_at)
        : report_id(report_id), proposal_id(proposal_id),
          validation_results(std::move(validation_results)),
          evidence_summary(std::move(evidence_summary)), known_risks(std::move(known_risks)),
          unknowns(std::move(unknowns)), generated_at(generated_at) {}
};
}
