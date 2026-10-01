#pragma once

#include "Version.h"

namespace xauusd::sovereign {

class MetricRegistryVersion {
public:
    MetricRegistryVersion() = default;
    explicit MetricRegistryVersion(Version v) : version_(v) {}

    const Version& version() const noexcept { return version_; }

    friend bool operator==(const MetricRegistryVersion&, const MetricRegistryVersion&) = default;
    friend bool operator!=(const MetricRegistryVersion&, const MetricRegistryVersion&) = default;

private:
    Version version_{};
};

} // namespace xauusd::sovereign
