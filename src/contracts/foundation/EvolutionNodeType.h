#pragma once

#include <cstdint>

namespace xauusd::sovereign {

enum class EvolutionNodeType : std::uint8_t {
    VERSION_NODE,
    HYPOTHESIS_NODE,
    EXPERIMENT_NODE,
    CANDIDATE_NODE,
    DECISION_NODE,
    INCIDENT_NODE
};

} // namespace xauusd::sovereign
