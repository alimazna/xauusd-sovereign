#pragma once
#include "Timestamp.h"
#include <chrono>
namespace xauusd::sovereign::detail {
inline Timestamp now_timestamp() noexcept {
    const auto us=std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
    return Timestamp{us.count()};
}
}
