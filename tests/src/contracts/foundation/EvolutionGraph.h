#pragma once

#include "EntityId.h"
#include "EvolutionEdge.h"
#include "Timestamp.h"

#include <vector>

namespace xauusd::sovereign {

struct EvolutionGraph {
    std::vector<EntityId>    node_ids;
    std::vector<EvolutionEdge> edges;
    Timestamp                built_at;
};

} // namespace xauusd::sovereign
