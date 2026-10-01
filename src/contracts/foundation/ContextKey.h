#pragma once

#include <string>
#include <utility>

namespace xauusd::sovereign {

class ContextKey {
public:
    ContextKey() = default;

    explicit ContextKey(std::string value)
        : value_(std::move(value)) {}

    const std::string& value() const noexcept {
        return value_;
    }

    friend bool operator==(const ContextKey&, const ContextKey&) = default;
    friend bool operator!=(const ContextKey&, const ContextKey&) = default;

    friend bool operator<(const ContextKey& a, const ContextKey& b) {
        return a.value_ < b.value_;
    }

private:
    std::string value_;
};

} // namespace xauusd::sovereign
