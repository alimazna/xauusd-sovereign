#pragma once

#include "Experiment.h"
#include "ExperimentFingerprint.h"

#include <cstdint>
#include <string>
#include <vector>

namespace xauusd::sovereign {

class FingerprintGenerator {
public:
    FingerprintGenerator() = default;

    ExperimentFingerprint generate(
        const Experiment& experiment) const;

    static std::vector<std::uint8_t> hash_components(
        const std::string& components);
};

} // namespace xauusd::sovereign
