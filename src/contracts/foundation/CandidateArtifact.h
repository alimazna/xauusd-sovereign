#pragma once

#include "Timestamp.h"

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace xauusd::sovereign {

struct CandidateArtifact {
    std::string               artifact_name;
    std::string               artifact_kind;
    std::vector<std::uint8_t> content_digest;
    std::uint64_t             content_size;
    Timestamp                 created_at;

    CandidateArtifact() = default;

    CandidateArtifact(
        std::string artifact_name,
        std::string artifact_kind,
        std::vector<std::uint8_t> content_digest,
        std::uint64_t content_size,
        Timestamp created_at)
        : artifact_name(std::move(artifact_name)),
          artifact_kind(std::move(artifact_kind)),
          content_digest(std::move(content_digest)),
          content_size(content_size),
          created_at(created_at) {}
};

} // namespace xauusd::sovereign
