#pragma once

#include "EntityId.h"
#include "EvidenceZone.h"
#include "Timestamp.h"

#include <string>
#include <utility>
#include <vector>

namespace xauusd::sovereign {

struct EvidenceLayer {
    EvidenceZone           zone;
    std::string            name;
    std::vector<EntityId>  dataset_ids;
    Timestamp              first_available_at;
    Timestamp              last_available_at;
    bool                   locked;

    EvidenceLayer() = default;

    EvidenceLayer(EvidenceZone zone, std::string name,
                  std::vector<EntityId> dataset_ids,
                  Timestamp first_available_at, Timestamp last_available_at,
                  bool locked)
        : zone(zone),
          name(std::move(name)),
          dataset_ids(std::move(dataset_ids)),
          first_available_at(first_available_at),
          last_available_at(last_available_at),
          locked(locked) {}
};

} // namespace xauusd::sovereign
