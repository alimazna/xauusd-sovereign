#pragma once
#include <cstdint>
namespace xauusd::sovereign {
enum class PromotionStatus : std::uint8_t {
    NOT_READY,
    READY_FOR_REVIEW,
    APPROVED_FOR_PROMOTION,
    PROMOTED,
    ABORTED,
    ROLLED_BACK
};
}
