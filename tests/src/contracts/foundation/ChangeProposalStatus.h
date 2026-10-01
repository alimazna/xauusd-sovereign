#pragma once
#include <cstdint>
namespace xauusd::sovereign {
enum class ChangeProposalStatus : std::uint8_t {
    DRAFTED,
    EVIDENCE_READY,
    UNDER_REVIEW,
    APPROVED,
    REJECTED,
    WITHDRAWN
};
}
