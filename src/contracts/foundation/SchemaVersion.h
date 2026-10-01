
#pragma once

#include "Version.h"

namespace xauusd::sovereign {

class SchemaVersion {
public:
    SchemaVersion() = default;

    explicit SchemaVersion(const Version& version)
        : version_(version) {}

    const Version& version() const noexcept {
        return version_;
    }

    friend bool operator==(const SchemaVersion&, const SchemaVersion&) = default;
    friend bool operator!=(const SchemaVersion&, const SchemaVersion&) = default;

private:
    Version version_{};
};

} // namespace xauusd::sovereign