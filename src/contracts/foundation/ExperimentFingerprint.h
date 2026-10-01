#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace xauusd::sovereign {

struct ExperimentFingerprint {
    std::vector<std::uint8_t> digest;
    std::string components;

    ExperimentFingerprint() = default;

    ExperimentFingerprint(
        std::vector<std::uint8_t> digest,
        std::string components)
        : digest(std::move(digest)),
          components(std::move(components)) {}

    friend bool operator==(const ExperimentFingerprint&, const ExperimentFingerprint&) = default;
    friend bool operator!=(const ExperimentFingerprint&, const ExperimentFingerprint&) = default;
};

} // namespace xauusd::sovereign
