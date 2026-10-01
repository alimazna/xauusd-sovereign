#pragma once

#include <cstdint>

namespace xauusd::sovereign {

enum class KnowledgeStatus : std::uint8_t {
    OBSERVED,
    SUSPECTED,
    UNDER_INVESTIGATION,
    SUPPORTED,
    VALIDATED,
    OPERATIONAL_KNOWLEDGE,
    REFUTED,
    CONTRADICTED,
    AGING
};

} // namespace xauusd::sovereign
