#pragma once

#include <cstdint>
#include <string>
#include <variant>

namespace xauusd::sovereign {

class ConfigurationValue {
public:
    using Storage = std::variant<
        bool,
        std::int64_t,
        double,
        std::string
    >;

    ConfigurationValue() = default;

    explicit ConfigurationValue(Storage storage)
        : storage_(std::move(storage)) {}

    const Storage& storage() const noexcept {
        return storage_;
    }

    friend bool operator==(const ConfigurationValue&,
                           const ConfigurationValue&) = default;

    friend bool operator!=(const ConfigurationValue&,
                           const ConfigurationValue&) = default;

private:
    Storage storage_{};
};

} // namespace xauusd::sovereign
