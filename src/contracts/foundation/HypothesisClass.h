#pragma once

#include <cstdint>

namespace xauusd::sovereign {

enum class HypothesisClass : std::uint8_t {
    FEATURE_HYPOTHESIS,
    PARAMETER_HYPOTHESIS,
    RULE_HYPOTHESIS,
    REGIME_HYPOTHESIS,
    TIMING_HYPOTHESIS,
    RISK_HYPOTHESIS,
    EXECUTION_HYPOTHESIS,
    INTERACTION_HYPOTHESIS,
    STRUCTURAL_HYPOTHESIS,
    DATA_QUALITY_HYPOTHESIS,
    RESEARCH_METHOD_HYPOTHESIS,
    UNKNOWN_HYPOTHESIS
};

} // namespace xauusd::sovereign
