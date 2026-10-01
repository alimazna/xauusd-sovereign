#pragma once

#include <cstdint>

namespace xauusd::sovereign {

enum class CandidateStatus : std::uint8_t {
    DRAFTED,
    SANDBOXED,
    VALIDATING,
    PASSED,
    FAILED,
    INCONCLUSIVE,
    CONTAMINATED,
    REVIEW_READY,
    HUMAN_APPROVED,
    HUMAN_REJECTED,
    SHADOW,
    PROMOTION_READY,
    DEPLOYED,
    ROLLED_BACK,
    RETIRED
};

} // namespace xauusd::sovereign
