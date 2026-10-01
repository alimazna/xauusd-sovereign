#include "FingerprintGenerator.h"

namespace xauusd::sovereign {

std::vector<std::uint8_t> FingerprintGenerator::hash_components(
    const std::string& components) {
    constexpr std::uint64_t offset_basis = 14695981039346656037ULL;
    constexpr std::uint64_t prime = 1099511628211ULL;

    std::uint64_t hash = offset_basis;
    for (const unsigned char value : components) {
        hash ^= static_cast<std::uint64_t>(value);
        hash *= prime;
    }

    std::vector<std::uint8_t> digest;
    digest.reserve(16U);
    for (int repeat = 0; repeat < 2; ++repeat) {
        for (int shift = 0; shift < 8; ++shift) {
            digest.push_back(static_cast<std::uint8_t>((hash >> (shift * 8)) & 0xffU));
        }
    }
    return digest;
}

ExperimentFingerprint FingerprintGenerator::generate(
    const Experiment& experiment) const {
    std::string components;
    components.reserve(128U);

    components += std::to_string(experiment.parent_version.value());
    components.push_back('|');
    components += experiment.dataset_version;
    components.push_back('|');
    components += experiment.feature_version;
    components.push_back('|');
    components += experiment.evaluator_version;
    components.push_back('|');
    components += experiment.environment_version;
    components.push_back('|');

    const auto& id_bytes = experiment.hypothesis_id.bytes();
    components.append(reinterpret_cast<const char*>(id_bytes.data()), id_bytes.size());

    return ExperimentFingerprint(hash_components(components), std::move(components));
}

} // namespace xauusd::sovereign
