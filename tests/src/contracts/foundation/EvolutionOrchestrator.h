#pragma once

#include "CandidateComparator.h"
#include "CandidateRegistry.h"
#include "EvolutionGraphBuilder.h"

#include <cstddef>
#include <string>
#include <vector>

namespace xauusd::sovereign {

class EvolutionOrchestrator {
public:
    EvolutionOrchestrator() = default;

    bool add_candidate(const Candidate& c);
    bool update_candidate_status(const EntityId& id, CandidateStatus s);
    std::size_t candidate_count() const noexcept;
    std::vector<Candidate> children_of(const EntityId& parent) const;

    bool register_node(const EntityId& id, EvolutionNodeType type);
    bool register_edge(const EntityId& from_id,
                       EvolutionNodeType from_type,
                       const EntityId& to_id,
                       EvolutionNodeType to_type,
                       const std::string& kind);
    EvolutionGraph build_graph(Timestamp at) const;
    std::size_t graph_node_count() const noexcept;
    std::size_t graph_edge_count() const noexcept;

    ComparisonResult compare_candidates(
        const EntityId& control_id,
        const EntityId& candidate_id,
        double control_metric,
        double candidate_metric) const;

    CandidateComparison compare_batch(
        const EntityId& control_id,
        const std::vector<EntityId>& candidate_ids,
        const std::vector<double>& candidate_metrics,
        double control_metric) const;

    void clear();

private:
    CandidateRegistry      registry_;
    EvolutionGraphBuilder  graph_builder_;
    CandidateComparator    comparator_;
};

} // namespace xauusd::sovereign
