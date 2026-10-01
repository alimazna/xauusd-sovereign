#pragma once

#include "ConfigurationKey.h"
#include "ConfigurationScope.h"
#include "ConfigurationValue.h"
#include "Version.h"

#include <map>

namespace xauusd::sovereign {

class ConfigurationSnapshot {
public:
    Version version;
    ConfigurationScope scope;
    std::map<ConfigurationKey, ConfigurationValue> entries;

    ConfigurationSnapshot() = default;

    ConfigurationSnapshot(
        Version version_value,
        ConfigurationScope scope_value,
        std::map<ConfigurationKey, ConfigurationValue> entries_value)
        : version(version_value),
          scope(scope_value),
          entries(std::move(entries_value)) {}
};

} // namespace xauusd::sovereign