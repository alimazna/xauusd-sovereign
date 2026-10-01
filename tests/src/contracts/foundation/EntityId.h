
#pragma once

#include <array>
#include <cstdint>

namespace xauusd::sovereign {

class EntityId {
public:
    EntityId() = default;

    explicit EntityId(const std::array<std::uint8_t, 16>& bytes)
        : bytes_(bytes) {}

    const std::array<std::uint8_t, 16>& bytes() const noexcept {
        return bytes_;
    }

    friend bool operator==(const EntityId&, const EntityId&) = default;
    friend bool operator!=(const EntityId&, const EntityId&) = default;

private:
    std::array<std::uint8_t, 16> bytes_{};
};

} // namespace xauusd::sovereign