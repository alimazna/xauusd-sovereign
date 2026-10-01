#pragma once

#include "EntityId.h"
#include "Timestamp.h"
#include "Version.h"
#include "CandidateType.h"
#include "CandidateStatus.h"
#include "CandidateArtifact.h"
#include "ExperimentFingerprint.h"

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace xauusd::sovereign {

struct Candidate {
    EntityId                       candidate_id;
    EntityId                       parent_candidate_id;
    EntityId                       hypothesis_id;
    CandidateType                  candidate_type;
    CandidateStatus                status;
    std::string                    change_summary;
    Version                        parent_version;
    Version                        candidate_version;
    ExperimentFingerprint          experiment_fingerprint;
    std::vector<CandidateArtifact> artifacts;
    double                         confidence;
    std::uint64_t                  trial_count;
    Timestamp                      created_at;
    Timestamp                      updated_at;
    Version                        creator_version;

    Candidate() = default;

    Candidate(
        EntityId candidate_id,
        EntityId parent_candidate_id,
        EntityId hypothesis_id,
        CandidateType candidate_type,
        CandidateStatus status,
        std::string change_summary,
        Version parent_version,
        Version candidate_version,
        ExperimentFingerprint experiment_fingerprint,
        std::vector<CandidateArtifact> artifacts,
        double confidence,
        std::uint64_t trial_count,
        Timestamp created_at,
        Timestamp updated_at,
        Version creator_version)
        : candidate_id(candidate_id),
          parent_candidate_id(parent_candidate_id),
          hypothesis_id(hypothesis_id),
          candidate_type(candidate_type),
          status(status),
          change_summary(std::move(change_summary)),
          parent_version(parent_version),
          candidate_version(candidate_version),
          experiment_fingerprint(std::move(experiment_fingerprint)),
          artifacts(std::move(artifacts)),
          confidence(confidence),
          trial_count(trial_count),
          created_at(created_at),
          updated_at(updated_at),
          creator_version(creator_version) {}
};

} // namespace xauusd::sovereign
