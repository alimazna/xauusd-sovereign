#pragma once

#include "EntityId.h"
#include "EvidenceZone.h"
#include "Timestamp.h"
#include "Version.h"

#include <utility>
#include <vector>

namespace xauusd::sovereign {

struct EvidenceSnapshot {
    EntityId              snapshot_id;
    EntityId              candidate_id;
    EvidenceZone          zone;
    std::vector<EntityId> dataset_ids;
    Version               metric_registry_version;
    Version               evaluator_version;
    Timestamp             taken_at;

    EvidenceSnapshot() = default;

    EvidenceSnapshot(EntityId snapshot_id, EntityId candidate_id, EvidenceZone zone,
                     std::vector<EntityId> dataset_ids,
                     Version metric_registry_version, Version evaluator_version,
                     Timestamp taken_at)
        : snapshot_id(snapshot_id),
          candidate_id(candidate_id),
          zone(zone),
          dataset_ids(std::move(dataset_ids)),
          metric_registry_version(metric_registry_version),
          evaluator_version(evaluator_version),
          taken_at(taken_at) {}
};

} // namespace xauusd::sovereign
