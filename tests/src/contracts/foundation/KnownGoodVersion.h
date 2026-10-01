#pragma once
#include "EntityId.h"
#include "Timestamp.h"
#include "Version.h"
#include <string>
#include <utility>
namespace xauusd::sovereign {
struct KnownGoodVersion {
    EntityId     known_good_id;
    Version      version;
    EntityId     approval_record_id;
    std::string  description;
    Timestamp    registered_at;
    bool         active;
    KnownGoodVersion() = default;
    KnownGoodVersion(EntityId known_good_id, Version version,
                     EntityId approval_record_id, std::string description,
                     Timestamp registered_at, bool active)
        : known_good_id(known_good_id), version(version),
          approval_record_id(approval_record_id), description(std::move(description)),
          registered_at(registered_at), active(active) {}
};
}
