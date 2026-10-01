#pragma once

#include "EntityId.h"
#include "Timestamp.h"

#include <string>
#include <utility>

namespace xauusd::sovereign {

struct KillSwitch {
    EntityId    switch_id;
    bool        engaged;
    std::string reason;
    Timestamp   engaged_at;
    std::string engaged_by;

    KillSwitch() = default;

    KillSwitch(EntityId switch_id, bool engaged, std::string reason,
               Timestamp engaged_at, std::string engaged_by)
        : switch_id(switch_id), engaged(engaged),
          reason(std::move(reason)), engaged_at(engaged_at),
          engaged_by(std::move(engaged_by)) {}
};

} // namespace xauusd::sovereign
