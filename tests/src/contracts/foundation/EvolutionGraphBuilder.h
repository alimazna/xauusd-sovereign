#pragma once

#include "EntityId.h"
#include "EvolutionEdge.h"
#include "EvolutionGraph.h"
#include "EvolutionNodeType.h"
#include "Timestamp.h"

#include <array>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace xauusd::sovereign {

class EvolutionGraphBuilder {
public:
    EvolutionGraphBuilder() = default;

    bool add_node(const EntityId& node_id, EvolutionNodeType type);
    bool add_edge(const EntityId& from_node_id,
                  EvolutionNodeType from_type,
                  const EntityId& to_node_id,
                  EvolutionNodeType to_type,
                  const std::string& edge_kind);

    std::vector<EvolutionEdge> edges_from(const EntityId& node_id) const;
    std::vector<EvolutionEdge> edges_to(const EntityId& node_id) const;
    bool has_node(const EntityId& node_id) const;

    EvolutionGraph build(Timestamp built_at) const;

    std::size_t node_count() const;
    std::size_t edge_count() const;

    void clear();

private:
    std::vector<EntityId> nodes_;
    std::map<std::array<std::uint8_t, 16>, std::size_t> node_index_;
    std::vector<EvolutionEdge> edges_;
};

} // namespace xauusd::sovereign
