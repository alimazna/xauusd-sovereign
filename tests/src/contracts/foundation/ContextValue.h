#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <variant>

namespace xauusd::sovereign {

class ContextValue {
public:
    using Storage = std::variant<bool, std::int64_t, double, std::string>;

    ContextValue() = default;

    explicit ContextValue(Storage v)
        : storage_(std::move(v)) {}

    const Storage& storage() const noexcept {
        return storage_;
    }

    friend bool operator==(const ContextValue&, const ContextValue&) = default;
    friend bool operator!=(const ContextValue&, const ContextValue&) = default;

private:
    Storage storage_;
};

} // namespace xauusd::sovereign
