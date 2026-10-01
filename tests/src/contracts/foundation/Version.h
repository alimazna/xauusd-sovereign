// src/contracts/foundation/Version.h
#pragma once

#include <cstdint>

namespace xauusd::sovereign {

class Version {
public:
    Version() = default;

    explicit Version(std::uint64_t value)
        : value_(value) {}

    std::uint64_t value() const noexcept {
        return value_;
    }

    friend bool operator==(const Version&, const Version&) = default;
    friend bool operator!=(const Version&, const Version&) = default;

private:
    std::uint64_t value_{};
};

} // namespace xauusd::sovereign