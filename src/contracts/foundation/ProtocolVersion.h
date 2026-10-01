
#pragma once

#include "Version.h"

namespace xauusd::sovereign {

class ProtocolVersion {
public:
    ProtocolVersion() = default;

    explicit ProtocolVersion(const Version& version)
        : version_(version) {}

    const Version& version() const noexcept {
        return version_;
    }

    friend bool operator==(const ProtocolVersion&, const ProtocolVersion&) = default;
    friend bool operator!=(const ProtocolVersion&, const ProtocolVersion&) = default;

private:
    Version version_{};
};

} // namespace xauusd::sovereign