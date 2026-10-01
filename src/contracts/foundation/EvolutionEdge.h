#pragma once

#include "EntityId.h"
#include "EvolutionNodeType.h"

#include <string>
#include <utility>

namespace xauusd::sovereign {

struct EvolutionEdge {
    EntityId          edge_id;
    EntityId          from_node_id;
    EntityId          to_node_id;
    EvolutionNodeType from_type;
    EvolutionNodeType to_type;
    std::string       edge_kind;

    EvolutionEdge() = default;

    EvolutionEdge(
        EntityId edge_id,
        EntityId from_node_id,
        EntityId to_node_id,
        EvolutionNodeType from_type,
        EvolutionNodeType to_type,
        std::string edge_kind)
        : edge_id(edge_id),
          from_node_id(from_node_id),
          to_node_id(to_node_id),
          from_type(from_type),
          to_type(to_type),
          edge_kind(std::move(edge_kind)) {}
};

} // namespace xauusd::sovereign
