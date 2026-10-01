#pragma once

#include "ConfigurationKey.h"
#include "ConfigurationValue.h"
#include "EntityId.h"
#include "Timestamp.h"

#include <string>
#include <utility>

namespace xauusd::sovereign {

class ConfigurationChange {
public:
    EntityId change_id;
    ConfigurationKey key;
    ConfigurationValue old_value;
    ConfigurationValue new_value;
    Timestamp timestamp;
    std::string actor;
    std::string reason;

    ConfigurationChange() = default;

    ConfigurationChange(
        EntityId change_id_value,
        ConfigurationKey key_value,
        ConfigurationValue old_value_value,
        ConfigurationValue new_value_value,
        Timestamp timestamp_value,
        std::string actor_value,
        std::string reason_value)
        : change_id(change_id_value),
          key(key_value),
          old_value(old_value_value),
          new_value(new_value_value),
          timestamp(timestamp_value),
          actor(std::move(actor_value)),
          reason(std::move(reason_value)) {}
};

} // namespace xauusd::sovereign