#pragma once

#include "EntityId.h"
#include "Timestamp.h"

#include <string>
#include <utility>
#include <vector>

namespace xauusd::sovereign {

struct SafetyCheck {
    EntityId              check_id;
    std::string           name;
    bool                  passed;
    std::string           failure_reason;
    Timestamp             performed_at;
    std::vector<EntityId> related_entities;

    SafetyCheck() = default;

    SafetyCheck(EntityId check_id, std::string name, bool passed,
                std::string failure_reason, Timestamp performed_at,
                std::vector<EntityId> related_entities)
        : check_id(check_id), name(std::move(name)), passed(passed),
          failure_reason(std::move(failure_reason)),
          performed_at(performed_at),
          related_entities(std::move(related_entities)) {}
};

} // namespace xauusd::sovereign
