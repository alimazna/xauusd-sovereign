#pragma once

#include <cstdint>
#include <vector>

namespace xauusd::sovereign {

class HashDigest {
public:
    HashDigest() = default;

    explicit HashDigest(std::vector<std::uint8_t> bytes)
        : bytes_(bytes) {}

    const std::vector<std::uint8_t>& bytes() const noexcept {
        return bytes_;
    }

    bool empty() const noexcept {
        return bytes_.empty();
    }

    friend bool operator==(const HashDigest& lhs, const HashDigest& rhs) {
        return lhs.bytes_ == rhs.bytes_;
    }

    friend bool operator!=(const HashDigest& lhs, const HashDigest& rhs) {
        return !(lhs == rhs);
    }

private:
    std::vector<std::uint8_t> bytes_;
};

} // namespace xauusd::sovereign