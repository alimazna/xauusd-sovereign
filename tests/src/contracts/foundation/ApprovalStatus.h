#pragma once
#include <cstdint>
namespace xauusd::sovereign {
enum class ApprovalStatus : std::uint8_t {
    PENDING,
    APPROVED,
    REJECTED,
    REQUEST_MORE_RESEARCH,
    MODIFY_PROPOSAL,
    FROZEN,
    EXPIRED
};
}
