#pragma once
#include "ICheckpointStore.h"
#include <array>
#include <cstdint>
#include <map>
#include <vector>
namespace xauusd::sovereign {
class CheckpointStore final : public ICheckpointStore {
public:
    CheckpointStore() = default;
    bool store(const Checkpoint& checkpoint) override;
    bool contains(const EntityId& checkpoint_id) const override;
    bool get(const EntityId& checkpoint_id, Checkpoint& out) const override;
    bool update_status(const EntityId& checkpoint_id, CheckpointStatus status) override;
    std::vector<Checkpoint> all() const override;
    std::size_t size() const override;
    void clear();
private:
    std::vector<Checkpoint> items_;
    std::map<std::array<std::uint8_t, 16>, std::size_t> index_;
};
}
