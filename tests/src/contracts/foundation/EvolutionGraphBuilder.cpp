#include "EvolutionGraphBuilder.h"

#include <utility>

namespace xauusd::sovereign {
namespace {

EntityId make_edge_id(const EntityId& from_node_id, const EntityId& to_node_id) {
    std::array<std::uint8_t, 16> bytes{};
    const auto& from = from_node_id.bytes();
    const auto& to = to_node_id.bytes();
    for (std::size_t i = 0; i < bytes.size(); ++i) {
        bytes[i] = static_cast<std::uint8_t>(from[i] ^ to[i]);
    }
    return EntityId{bytes};
}

} // namespace

bool EvolutionGraphBuilder::add_node(const EntityId& node_id, EvolutionNodeType type) {
    (void)type;
    const auto key = node_id.bytes();
    if (node_index_.find(key) != node_index_.end()) {
        return false;
    }

    nodes_.push_back(node_id);
    node_index_.emplace(key, nodes_.size() - 1U);
    return true;
}

bool EvolutionGraphBuilder::add_edge(const EntityId& from_node_id,
                                     EvolutionNodeType from_type,
                                     const EntityId& to_node_id,
                                     EvolutionNodeType to_type,
                                     const std::string& edge_kind) {
    if (!has_node(from_node_id) || !has_node(to_node_id)) {
        return false;
    }

    const EntityId edge_id = make_edge_id(from_node_id, to_node_id);
    for (const auto& edge : edges_) {
        if (edge.edge_id == edge_id &&
            edge.from_node_id == from_node_id &&
            edge.to_node_id == to_node_id) {
            return false;
        }
    }

    edges_.emplace_back(edge_id,
                        from_node_id,
                        to_node_id,
                        from_type,
                        to_type,
                        edge_kind);
    return true;
}

std::vector<EvolutionEdge> EvolutionGraphBuilder::edges_from(const EntityId& node_id) const {
    std::vector<EvolutionEdge> result;
    for (const auto& edge : edges_) {
        if (edge.from_node_id == node_id) {
            result.push_back(edge);
        }
    }
    return result;
}

std::vector<EvolutionEdge> EvolutionGraphBuilder::edges_to(const EntityId& node_id) const {
    std::vector<EvolutionEdge> result;
    for (const auto& edge : edges_) {
        if (edge.to_node_id == node_id) {
            result.push_back(edge);
        }
    }
    return result;
}

bool EvolutionGraphBuilder::has_node(const EntityId& node_id) const {
    return node_index_.find(node_id.bytes()) != node_index_.end();
}

EvolutionGraph EvolutionGraphBuilder::build(Timestamp built_at) const {
    EvolutionGraph graph;
    graph.node_ids = nodes_;
    graph.edges = edges_;
    graph.built_at = built_at;
    return graph;
}

std::size_t EvolutionGraphBuilder::node_count() const {
    return nodes_.size();
}

std::size_t EvolutionGraphBuilder::edge_count() const {
    return edges_.size();
}

void EvolutionGraphBuilder::clear() {
    nodes_.clear();
    node_index_.clear();
    edges_.clear();
}

} // namespace xauusd::sovereign
