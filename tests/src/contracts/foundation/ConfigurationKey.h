#pragma once

#include <string>

namespace xauusd::sovereign {

class ConfigurationKey {
public:
    ConfigurationKey() = default;

    explicit ConfigurationKey(std::string value)
        : value_(std::move(value)) {}

    const std::string& value() const noexcept {
        return value_;
    }

    friend bool operator==(const ConfigurationKey&, const ConfigurationKey&) = default;
    friend bool operator!=(const ConfigurationKey&, const ConfigurationKey&) = default;

    friend bool operator<(const ConfigurationKey& lhs,
                          const ConfigurationKey& rhs) {
        return lhs.value_ < rhs.value_;
    }

private:
    std::string value_;
};

} // namespace xauusd::sovereign