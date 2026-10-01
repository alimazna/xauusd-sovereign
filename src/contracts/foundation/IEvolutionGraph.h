#pragma once

#include "EntityId.h"
#include "EvolutionEdge.h"
#include "EvolutionGraph.h"
#include "EvolutionNodeType.h"

#include <cstddef>
#include <vector>

namespace xauusd::sovereign {

class IEvolutionGraph {
public:
    virtual ~IEvolutionGraph() = default;

    virtual bool add_node(const EntityId& node_id,
                          EvolutionNodeType type) = 0;
    virtual bool add_edge(const EvolutionEdge& edge) = 0;
    virtual std::vector<EvolutionEdge> edges_from(const EntityId& node_id) const = 0;
    virtual std::vector<EvolutionEdge> edges_to(const EntityId& node_id) const = 0;
    virtual bool has_node(const EntityId& node_id) const = 0;
    virtual std::size_t node_count() const = 0;
    virtual std::size_t edge_count() const = 0;
};

} // namespace xauusd::sovereign
