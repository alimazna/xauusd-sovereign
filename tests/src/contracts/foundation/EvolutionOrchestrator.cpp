#include "EvolutionOrchestrator.h"

namespace xauusd::sovereign {

bool EvolutionOrchestrator::add_candidate(const Candidate& c) {
    return registry_.add(c);
}

bool EvolutionOrchestrator::update_candidate_status(const EntityId& id, CandidateStatus s) {
    return registry_.update_status(id, s);
}

std::size_t EvolutionOrchestrator::candidate_count() const noexcept {
    return registry_.size();
}

std::vector<Candidate> EvolutionOrchestrator::children_of(const EntityId& parent) const {
    return registry_.children_of(parent);
}

bool EvolutionOrchestrator::register_node(const EntityId& id, EvolutionNodeType type) {
    return graph_builder_.add_node(id, type);
}

bool EvolutionOrchestrator::register_edge(const EntityId& from_id,
                                          EvolutionNodeType from_type,
                                          const EntityId& to_id,
                                          EvolutionNodeType to_type,
                                          const std::string& kind) {
    return graph_builder_.add_edge(from_id, from_type, to_id, to_type, kind);
}

EvolutionGraph EvolutionOrchestrator::build_graph(Timestamp at) const {
    return graph_builder_.build(at);
}

std::size_t EvolutionOrchestrator::graph_node_count() const noexcept {
    return graph_builder_.node_count();
}

std::size_t EvolutionOrchestrator::graph_edge_count() const noexcept {
    return graph_builder_.edge_count();
}

ComparisonResult EvolutionOrchestrator::compare_candidates(const EntityId& control_id,
                                                           const EntityId& candidate_id,
                                                           double control_metric,
                                                           double candidate_metric) const {
    return comparator_.compare(control_id, candidate_id, control_metric, candidate_metric);
}

CandidateComparison EvolutionOrchestrator::compare_batch(
    const EntityId& control_id,
    const std::vector<EntityId>& candidate_ids,
    const std::vector<double>& candidate_metrics,
    double control_metric) const {
    return comparator_.compare_all(control_id, candidate_ids, candidate_metrics, control_metric);
}

void EvolutionOrchestrator::clear() {
    registry_.clear();
    graph_builder_.clear();
}

} // namespace xauusd::sovereign
