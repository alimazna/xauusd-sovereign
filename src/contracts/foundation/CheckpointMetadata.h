#pragma once
#include "EntityId.h"
#include "Timestamp.h"
#include "Version.h"
#include <string>
#include <utility>
namespace xauusd::sovereign {
struct CheckpointMetadata {
    EntityId     checkpoint_id;
    EntityId     related_experiment_id;
    Version      schema_version;
    Timestamp    created_at;
    std::string  description;
    CheckpointMetadata() = default;
    CheckpointMetadata(EntityId checkpoint_id, EntityId related_experiment_id,
                       Version schema_version, Timestamp created_at,
                       std::string description)
        : checkpoint_id(checkpoint_id), related_experiment_id(related_experiment_id),
          schema_version(schema_version), created_at(created_at), description(std::move(description)) {}
};
}
