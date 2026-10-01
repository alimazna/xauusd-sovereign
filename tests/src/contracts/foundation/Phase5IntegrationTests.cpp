#include "Candidate.h"
#include "CandidateComparator.h"
#include "CandidateRegistry.h"
#include "EvolutionGraphBuilder.h"
#include "EvolutionOrchestrator.h"

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#define CHECK(condition) \
    do { \
        if (!(condition)) { \
            return false; \
        } \
    } while (false)

namespace xauusd::sovereign {
namespace {

static EntityId make_id(std::uint8_t seed) {
    std::array<std::uint8_t, 16> bytes{};
    bytes[0] = seed;
    return EntityId{bytes};
}

static Candidate make_candidate(std::uint8_t seed,
                                const EntityId& parent = EntityId{}) {
    return Candidate{
        make_id(seed),
        parent,
        make_id(static_cast<std::uint8_t>(seed + 20U)),
        CandidateType::RULE_CANDIDATE,
        CandidateStatus::DRAFTED,
        "change-" + std::to_string(seed),
        Version{1U},
        Version{2U},
        ExperimentFingerprint{{seed}, "components"},
        {CandidateArtifact{"artifact", "test", {seed}, 1U, Timestamp{seed}}},
        0.5,
        0U,
        Timestamp{seed},
        Timestamp{seed},
        Version{1U}};
}

bool test_candidate_registry_add_contains_size() {
    CandidateRegistry registry;
    const Candidate candidate = make_candidate(1U);
    CHECK(registry.add(candidate));
    CHECK(registry.contains(candidate.candidate_id));
    CHECK(registry.size() == 1U);
    return true;
}

bool test_candidate_registry_duplicate_rejection() {
    CandidateRegistry registry;
    const Candidate candidate = make_candidate(2U);
    return registry.add(candidate) && !registry.add(candidate) && registry.size() == 1U;
}

bool test_candidate_registry_get_update_status() {
    CandidateRegistry registry;
    const Candidate candidate = make_candidate(3U);
    Candidate out;
    if (!registry.add(candidate) || !registry.get(candidate.candidate_id, out)) {
        return false;
    }
    if (out.status != CandidateStatus::DRAFTED) {
        return false;
    }
    if (!registry.update_status(candidate.candidate_id, CandidateStatus::SANDBOXED)) {
        return false;
    }
    return registry.get(candidate.candidate_id, out) &&
           out.status == CandidateStatus::SANDBOXED;
}

bool test_candidate_registry_all_clear() {
    CandidateRegistry registry;
    registry.add(make_candidate(4U));
    registry.add(make_candidate(5U));
    return registry.all().size() == 2U && (registry.clear(), registry.size() == 0U);
}

bool test_candidate_registry_children_of() {
    CandidateRegistry registry;
    const EntityId parent = make_id(6U);
    registry.add(make_candidate(7U, parent));
    registry.add(make_candidate(8U, parent));
    registry.add(make_candidate(9U));
    return registry.children_of(parent).size() == 2U;
}

bool test_candidate_enum_sanity() {
    return static_cast<std::uint8_t>(CandidateType::PARAMETER_CANDIDATE) == 0U &&
           static_cast<std::uint8_t>(CandidateType::UNKNOWN_CANDIDATE) == 7U &&
           static_cast<std::uint8_t>(CandidateStatus::DRAFTED) == 0U &&
           static_cast<std::uint8_t>(CandidateStatus::RETIRED) == 14U;
}

bool test_candidate_artifact_basic_fields() {
    const CandidateArtifact artifact{"name", "kind", {1U, 2U}, 2U, Timestamp{10}};
    return artifact.artifact_name == "name" && artifact.artifact_kind == "kind" &&
           artifact.content_digest.size() == 2U && artifact.content_size == 2U &&
           artifact.created_at == Timestamp{10};
}

bool test_graph_add_node_has_node() {
    EvolutionGraphBuilder graph;
    const EntityId id = make_id(10U);
    return graph.add_node(id, EvolutionNodeType::CANDIDATE_NODE) &&
           graph.has_node(id) && graph.node_count() == 1U;
}

bool test_graph_add_edge_count() {
    EvolutionGraphBuilder graph;
    const EntityId from = make_id(11U);
    const EntityId to = make_id(12U);
    graph.add_node(from, EvolutionNodeType::VERSION_NODE);
    graph.add_node(to, EvolutionNodeType::CANDIDATE_NODE);
    return graph.add_edge(from, EvolutionNodeType::VERSION_NODE, to,
                          EvolutionNodeType::CANDIDATE_NODE, "derived") &&
           graph.edge_count() == 1U;
}

bool test_graph_edges_from() {
    EvolutionGraphBuilder graph;
    const EntityId from = make_id(13U);
    const EntityId to = make_id(14U);
    graph.add_node(from, EvolutionNodeType::VERSION_NODE);
    graph.add_node(to, EvolutionNodeType::CANDIDATE_NODE);
    graph.add_edge(from, EvolutionNodeType::VERSION_NODE, to,
                   EvolutionNodeType::CANDIDATE_NODE, "derived");
    const auto edges = graph.edges_from(from);
    return edges.size() == 1U && edges[0].to_node_id == to;
}

bool test_graph_edges_to() {
    EvolutionGraphBuilder graph;
    const EntityId from = make_id(15U);
    const EntityId to = make_id(16U);
    graph.add_node(from, EvolutionNodeType::VERSION_NODE);
    graph.add_node(to, EvolutionNodeType::CANDIDATE_NODE);
    graph.add_edge(from, EvolutionNodeType::VERSION_NODE, to,
                   EvolutionNodeType::CANDIDATE_NODE, "derived");
    const auto edges = graph.edges_to(to);
    return edges.size() == 1U && edges[0].from_node_id == from;
}

bool test_graph_build_counts() {
    EvolutionGraphBuilder graph;
    const EntityId a = make_id(17U);
    const EntityId b = make_id(18U);
    graph.add_node(a, EvolutionNodeType::VERSION_NODE);
    graph.add_node(b, EvolutionNodeType::CANDIDATE_NODE);
    graph.add_edge(a, EvolutionNodeType::VERSION_NODE, b,
                   EvolutionNodeType::CANDIDATE_NODE, "derived");
    const EvolutionGraph built = graph.build(Timestamp{99});
    return built.node_ids.size() == 2U && built.edges.size() == 1U &&
           built.built_at == Timestamp{99};
}

bool test_comparator_better() {
    CandidateComparator comparator;
    const auto result = comparator.compare(make_id(19U), make_id(20U), 1.0, 2.0);
    return result.outcome == ComparisonOutcome::BETTER && result.primary_delta == 1.0;
}

bool test_comparator_worse() {
    CandidateComparator comparator;
    const auto result = comparator.compare(make_id(21U), make_id(22U), 2.0, 1.0);
    return result.outcome == ComparisonOutcome::WORSE && result.primary_delta == -1.0;
}

bool test_comparator_equal() {
    CandidateComparator comparator;
    const auto result = comparator.compare(make_id(23U), make_id(24U), 1.0, 1.0 + 5e-10);
    return result.outcome == ComparisonOutcome::EQUAL;
}

bool test_comparator_compare_all() {
    CandidateComparator comparator;
    const std::vector<EntityId> ids{make_id(25U), make_id(26U), make_id(27U)};
    const std::vector<double> metrics{2.0, 1.0, 3.0};
    const auto comparison = comparator.compare_all(make_id(28U), ids, metrics, 1.5);
    return comparison.results.size() == 3U &&
           comparison.results[0].outcome == ComparisonOutcome::BETTER &&
           comparison.results[1].outcome == ComparisonOutcome::WORSE &&
           comparison.results[2].outcome == ComparisonOutcome::BETTER &&
           !comparison.summary.empty();
}

bool test_orchestrator_candidate_flow() {
    EvolutionOrchestrator orchestrator;
    const Candidate candidate = make_candidate(29U);
    return orchestrator.add_candidate(candidate) &&
           orchestrator.candidate_count() == 1U &&
           orchestrator.update_candidate_status(candidate.candidate_id,
                                                CandidateStatus::REVIEW_READY) &&
           orchestrator.children_of(candidate.parent_candidate_id).size() == 1U;
}

bool test_orchestrator_graph_flow() {
    EvolutionOrchestrator orchestrator;
    const EntityId a = make_id(30U);
    const EntityId b = make_id(31U);
    if (!orchestrator.register_node(a, EvolutionNodeType::VERSION_NODE) ||
        !orchestrator.register_node(b, EvolutionNodeType::CANDIDATE_NODE) ||
        !orchestrator.register_edge(a, EvolutionNodeType::VERSION_NODE, b,
                                    EvolutionNodeType::CANDIDATE_NODE, "derived")) {
        return false;
    }
    const auto graph = orchestrator.build_graph(Timestamp{101});
    return orchestrator.graph_node_count() == 2U &&
           orchestrator.graph_edge_count() == 1U &&
           graph.built_at == Timestamp{101};
}

bool test_orchestrator_comparison_flow() {
    EvolutionOrchestrator orchestrator;
    const auto result = orchestrator.compare_candidates(make_id(32U), make_id(33U), 1.0, 2.0);
    const auto batch = orchestrator.compare_batch(make_id(32U),
                                                   {make_id(33U), make_id(34U)},
                                                   {2.0, 0.5},
                                                   1.0);
    return result.outcome == ComparisonOutcome::BETTER && batch.results.size() == 2U;
}

bool test_orchestrator_clear() {
    EvolutionOrchestrator orchestrator;
    const Candidate candidate = make_candidate(35U);
    const EntityId a = make_id(36U);
    const EntityId b = make_id(37U);
    orchestrator.add_candidate(candidate);
    orchestrator.register_node(a, EvolutionNodeType::VERSION_NODE);
    orchestrator.register_node(b, EvolutionNodeType::CANDIDATE_NODE);
    orchestrator.register_edge(a, EvolutionNodeType::VERSION_NODE, b,
                               EvolutionNodeType::CANDIDATE_NODE, "derived");
    orchestrator.clear();
    return orchestrator.candidate_count() == 0U &&
           orchestrator.graph_node_count() == 0U &&
           orchestrator.graph_edge_count() == 0U;
}

} // namespace
} // namespace xauusd::sovereign

namespace {

using xauusd::sovereign::test_candidate_artifact_basic_fields;
using xauusd::sovereign::test_candidate_enum_sanity;
using xauusd::sovereign::test_candidate_registry_add_contains_size;
using xauusd::sovereign::test_candidate_registry_all_clear;
using xauusd::sovereign::test_candidate_registry_children_of;
using xauusd::sovereign::test_candidate_registry_duplicate_rejection;
using xauusd::sovereign::test_candidate_registry_get_update_status;
using xauusd::sovereign::test_comparator_better;
using xauusd::sovereign::test_comparator_compare_all;
using xauusd::sovereign::test_comparator_equal;
using xauusd::sovereign::test_comparator_worse;
using xauusd::sovereign::test_graph_add_edge_count;
using xauusd::sovereign::test_graph_add_node_has_node;
using xauusd::sovereign::test_graph_build_counts;
using xauusd::sovereign::test_graph_edges_from;
using xauusd::sovereign::test_graph_edges_to;
using xauusd::sovereign::test_orchestrator_candidate_flow;
using xauusd::sovereign::test_orchestrator_clear;
using xauusd::sovereign::test_orchestrator_comparison_flow;
using xauusd::sovereign::test_orchestrator_graph_flow;

int run_tests() {
    const bool tests[] = {
        test_candidate_registry_add_contains_size(),
        test_candidate_registry_duplicate_rejection(),
        test_candidate_registry_get_update_status(),
        test_candidate_registry_all_clear(),
        test_candidate_registry_children_of(),
        test_candidate_enum_sanity(),
        test_candidate_artifact_basic_fields(),
        test_graph_add_node_has_node(),
        test_graph_add_edge_count(),
        test_graph_edges_from(),
        test_graph_edges_to(),
        test_graph_build_counts(),
        test_comparator_better(),
        test_comparator_worse(),
        test_comparator_equal(),
        test_comparator_compare_all(),
        test_orchestrator_candidate_flow(),
        test_orchestrator_graph_flow(),
        test_orchestrator_comparison_flow(),
        test_orchestrator_clear()};

    for (const bool passed : tests) {
        if (!passed) {
            return 1;
        }
    }
    return 0;
}

} // namespace

int main() {
    return run_tests();
}
