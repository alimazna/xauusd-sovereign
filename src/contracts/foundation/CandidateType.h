#pragma once

#include <cstdint>

namespace xauusd::sovereign {

enum class CandidateType : std::uint8_t {
    PARAMETER_CANDIDATE,
    RULE_CANDIDATE,
    FEATURE_CANDIDATE,
    REGIME_CANDIDATE,
    STRATEGY_CANDIDATE,
    RESEARCH_METHOD_CANDIDATE,
    ARCHITECTURE_CANDIDATE,
    UNKNOWN_CANDIDATE
};

} // namespace xauusd::sovereign
