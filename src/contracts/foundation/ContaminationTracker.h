#pragma once

#include "EntityId.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <map>

namespace xauusd::sovereign {

enum class ContaminationState : std::uint8_t {
    CLEAN,
    VALIDATED,
    OOS_EXPOSED,
    HOLDOUT_EXPOSED,
    CONTAMINATED,
    RETIRED
};

class ContaminationTracker {
public:
    ContaminationTracker() = default;

    bool mark(const EntityId& candidate_id, ContaminationState state);
    bool get(const EntityId& candidate_id, ContaminationState& out) const;
    bool contains(const EntityId& candidate_id) const;
    std::size_t size() const;
    void clear();

private:
    using Key = std::array<std::uint8_t, 16>;

    static Key key_of(const EntityId& id);

    std::map<Key, ContaminationState> states_;
};

} // namespace xauusd::sovereign
