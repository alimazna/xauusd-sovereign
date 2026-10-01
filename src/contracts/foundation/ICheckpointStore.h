#pragma once
#include "Checkpoint.h"
#include "CheckpointStatus.h"
#include "EntityId.h"
#include <cstddef>
#include <vector>
namespace xauusd::sovereign {
class ICheckpointStore {
public:
    virtual ~ICheckpointStore() = default;
    virtual bool store(const Checkpoint& checkpoint) = 0;
    virtual bool contains(const EntityId& checkpoint_id) const = 0;
    virtual bool get(const EntityId& checkpoint_id, Checkpoint& out) const = 0;
    virtual bool update_status(const EntityId& checkpoint_id, CheckpointStatus status) = 0;
    virtual std::vector<Checkpoint> all() const = 0;
    virtual std::size_t size() const = 0;
};
}
