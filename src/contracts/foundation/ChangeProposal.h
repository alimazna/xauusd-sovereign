#pragma once
#include "Candidate.h"
#include "ChangeProposalStatus.h"
#include "EntityId.h"
#include "Timestamp.h"
#include "Version.h"
#include <string>
#include <utility>
namespace xauusd::sovereign {
struct ChangeProposal {
    EntityId              proposal_id;
    EntityId              candidate_id;
    ChangeProposalStatus  status;
    std::string           title;
    std::string           problem_summary;
    std::string           change_summary;
    std::string           expected_effect;
    std::string           risks;
    Version               current_version;
    Version               candidate_version;
    Timestamp             created_at;
    Timestamp             updated_at;
    ChangeProposal() = default;
    ChangeProposal(EntityId proposal_id, EntityId candidate_id,
                   ChangeProposalStatus status, std::string title,
                   std::string problem_summary, std::string change_summary,
                   std::string expected_effect, std::string risks,
                   Version current_version, Version candidate_version,
                   Timestamp created_at, Timestamp updated_at)
        : proposal_id(proposal_id), candidate_id(candidate_id), status(status),
          title(std::move(title)), problem_summary(std::move(problem_summary)),
          change_summary(std::move(change_summary)), expected_effect(std::move(expected_effect)),
          risks(std::move(risks)), current_version(current_version),
          candidate_version(candidate_version), created_at(created_at), updated_at(updated_at) {}
};
}
