// src/contracts/foundation/Timestamp.h
#pragma once

#include <cstdint>

namespace xauusd::sovereign {

class Timestamp {
public:
    Timestamp() = default;

    explicit Timestamp(std::int64_t value)
        : value_(value) {}

    std::int64_t value() const noexcept {
        return value_;
    }

    friend bool operator==(const Timestamp&, const Timestamp&) = default;
    friend bool operator!=(const Timestamp&, const Timestamp&) = default;

private:
    std::int64_t value_{};
};

} // namespace xauusd::sovereign