#include "KnowledgeStore.h"
#include "KnowledgeLifecycle.h"
#include "ContradictionEngine.h"
#include "FailureMemory.h"
#include "ContextLearningEngine.h"
#include "LearningOrchestrator.h"

#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <variant>
#include <vector>

using namespace xauusd::sovereign;

static int g_failures = 0;

#define CHECK(expr) do {                                              \
    if (!(expr)) {                                                    \
        std::cerr << "FAIL: " #expr                                   \
                  << " at line " << __LINE__ << "\n";                 \
        ++g_failures;                                                 \
    }                                                                 \
} while (0)

static EntityId make_id(std::uint8_t seed) {
    std::array<std::uint8_t, 16> bytes{};
    bytes[0] = seed;
    return EntityId{bytes};
}

static KnowledgeId make_kid(std::uint8_t seed) {
    return KnowledgeId{make_id(seed)};
}

static KnowledgeObject make_knowledge(
    std::uint8_t seed,
    const std::string& conclusion = "increase") {
    std::vector<EntityId> refs{make_id(static_cast<std::uint8_t>(seed + 10U))};

    return KnowledgeObject{
        make_kid(seed),
        "observation-" + std::to_string(seed),
        "context-" + std::to_string(seed),
        conclusion,
        "scope-1",
        KnowledgeStatus::OBSERVED,
        0.8,
        Timestamp{100},
        Timestamp{100},
        Timestamp{1000},
        refs,
        {},
        {},
        {},
        {},
        {},
        {}};
}

static FailureMemoryEntry make_failure(std::uint8_t seed) {
    const EntityId pattern_id = make_id(seed);
    FailurePattern pattern{
        pattern_id,
        FailureType::SIGNAL_FAILURE,
        Timestamp{10},
        Timestamp{20},
        2,
        "market-context",
        "signal failed"};

    return FailureMemoryEntry{
        make_id(static_cast<std::uint8_t>(seed + 1U)),
        pattern,
        {},
        {"re-check input"},
        2,
        Timestamp{10},
        Timestamp{20}};
}

int main() {
    // 1. KnowledgeStore add/contains/size
    KnowledgeStore store;
    const auto object1 = make_knowledge(1);
    CHECK(store.add(object1));
    CHECK(store.contains(object1.knowledge_id));
    CHECK(store.size() == 1U);

    // 2. KnowledgeStore duplicate rejection
    CHECK(!store.add(object1));
    CHECK(store.size() == 1U);

    // 3. KnowledgeStore get/update
    KnowledgeObject fetched;
    CHECK(store.get(object1.knowledge_id, fetched));
    CHECK(fetched.knowledge_id == object1.knowledge_id);
    auto object1_updated = object1;
    object1_updated.confidence = 0.9;
    CHECK(store.update(object1_updated));
    CHECK(store.get(object1_updated.knowledge_id, fetched));
    CHECK(fetched.confidence == 0.9);

    // 4. KnowledgeStore all/clear
    CHECK(store.all().size() == 1U);
    store.clear();
    CHECK(store.size() == 0U);

    // 5. KnowledgeId wrapper
    const auto entity5 = make_id(5);
    const KnowledgeId knowledge5{entity5};
    CHECK(knowledge5.entity_id() == entity5);
    CHECK(knowledge5 == KnowledgeId{entity5});
    CHECK(knowledge5 != make_kid(6));

    // 6. KnowledgeStatus enum
    CHECK(static_cast<std::uint8_t>(KnowledgeStatus::OBSERVED) == 0U);
    CHECK(static_cast<std::uint8_t>(KnowledgeStatus::AGING) == 8U);

    // 7. Lifecycle transition recording
    KnowledgeLifecycle lifecycle;
    const KnowledgeTransition transition{
        make_kid(7),
        KnowledgeStatus::OBSERVED,
        KnowledgeStatus::SUPPORTED,
        Timestamp{200},
        "supported by outcome"};
    CHECK(lifecycle.transition(transition));

    // 8. Lifecycle history_for
    const auto history = lifecycle.history_for(make_kid(7));
    CHECK(history.size() == 1U);
    CHECK(history[0].transition.reason == "supported by outcome");

    // 9. Lifecycle transition_count
    CHECK(lifecycle.transition_count() == 1U);

    // 10. KnowledgeContradiction register
    const KnowledgeContradiction contradiction{
        make_id(20),
        make_kid(7),
        make_kid(8),
        "scope-1",
        Timestamp{210},
        false};
    CHECK(lifecycle.register_contradiction(contradiction));
    CHECK(!lifecycle.register_contradiction(contradiction));

    // 11. ContradictionEngine detect_conflicts
    ContradictionEngine contradiction_engine;
    const auto conflict_a = make_knowledge(21, "increase");
    const auto conflict_b = make_knowledge(22, "decrease");
    const auto conflicts = contradiction_engine.detect_conflicts({conflict_a, conflict_b});
    CHECK(conflicts.size() == 1U);
    CHECK(conflicts[0].knowledge_a == conflict_a.knowledge_id);
    CHECK(conflicts[0].knowledge_b == conflict_b.knowledge_id);
    CHECK(!conflicts[0].resolved);
    CHECK(contradiction_engine.resolve(conflicts[0].contradiction_id));
    CHECK(!contradiction_engine.resolve(conflicts[0].contradiction_id));

    // 12. FailureMemory record/get
    FailureMemory failure_memory;
    const auto failure_entry = make_failure(30);
    CHECK(failure_memory.record(failure_entry));
    FailureMemoryEntry failure_out;
    CHECK(failure_memory.get(failure_entry.entry_id, failure_out));
    CHECK(failure_out.pattern.failure_type == FailureType::SIGNAL_FAILURE);

    // 13. FailureMemory add_hypothesis/hypotheses_for
    const RCAHypothesis hypothesis{
        make_id(31),
        failure_entry.pattern.pattern_id,
        "feature mismatch",
        0.7,
        false,
        false,
        Timestamp{25}};
    CHECK(failure_memory.add_hypothesis(hypothesis));
    const auto hypotheses = failure_memory.hypotheses_for(failure_entry.pattern.pattern_id);
    CHECK(hypotheses.size() == 1U);
    CHECK(hypotheses[0].hypothesis_id == hypothesis.hypothesis_id);

    // 14. FailureMemory confirm/reject
    CHECK(failure_memory.confirm_hypothesis(hypothesis.hypothesis_id));
    auto hypotheses_after_confirm =
        failure_memory.hypotheses_for(failure_entry.pattern.pattern_id);
    CHECK(hypotheses_after_confirm[0].confirmed);
    CHECK(!hypotheses_after_confirm[0].rejected);
    CHECK(failure_memory.reject_hypothesis(hypothesis.hypothesis_id));
    auto hypotheses_after_reject =
        failure_memory.hypotheses_for(failure_entry.pattern.pattern_id);
    CHECK(!hypotheses_after_reject[0].confirmed);
    CHECK(hypotheses_after_reject[0].rejected);

    // 15. ContextLearningEngine learn/query
    ContextLearningEngine context_engine;
    const ContextKey key{"regime"};
    const ContextLearningRecord record{
        make_id(40),
        key,
        ContextValue{ContextValue::Storage{std::string{"trend"}}},
        "trend context observed",
        0.6,
        Timestamp{300}};
    CHECK(context_engine.learn(record));
    CHECK(context_engine.size() == 1U);
    const auto queried = context_engine.query(key);
    CHECK(queried.size() == 1U);
    CHECK(queried[0].observation == "trend context observed");

    // 16. ContextKey equality/ordering
    const ContextKey key_same{"regime"};
    const ContextKey key_other{"volatility"};
    CHECK(key == key_same);
    CHECK(key != key_other);
    CHECK((key < key_other) || (key_other < key));

    // 17. ContextValue variant
    const ContextValue bool_value{ContextValue::Storage{true}};
    const ContextValue int_value{ContextValue::Storage{std::int64_t{42}}};
    const ContextValue double_value{ContextValue::Storage{0.25}};
    const ContextValue string_value{ContextValue::Storage{std::string{"x"}}};
    CHECK(std::get<bool>(bool_value.storage()));
    CHECK(std::get<std::int64_t>(int_value.storage()) == 42);
    CHECK(std::get<double>(double_value.storage()) == 0.25);
    CHECK(std::get<std::string>(string_value.storage()) == "x");

    // 18. LearningOrchestrator end-to-end
    LearningOrchestrator orchestrator;
    const auto orch_object = make_knowledge(50);
    CHECK(orchestrator.add_knowledge(orch_object));
    CHECK(orchestrator.update_knowledge(
        [&]() {
            auto updated = orch_object;
            updated.status = KnowledgeStatus::SUPPORTED;
            return updated;
        }()));
    CHECK(orchestrator.knowledge_count() == 1U);
    CHECK(orchestrator.transition_knowledge(transition));
    CHECK(orchestrator.knowledge_history(make_kid(7)).size() == 1U);
    CHECK(orchestrator.record_failure(failure_entry));
    CHECK(orchestrator.add_hypothesis(hypothesis));
    CHECK(orchestrator.failure_count() == 1U);
    CHECK(orchestrator.learn_context(record));
    CHECK(orchestrator.context_count() == 1U);

    // 19. LearningOrchestrator clear
    orchestrator.clear();
    CHECK(orchestrator.knowledge_count() == 0U);
    CHECK(orchestrator.failure_count() == 0U);
    CHECK(orchestrator.context_count() == 0U);

    // 20. Decay evaluate
    const auto decay_object = make_knowledge(60);
    const DecayPolicy policy{
        2'000,
        1'000,
        0.5,
        0.2};
    const auto evaluation =
        orchestrator.evaluate_decay(decay_object, policy, Timestamp{1'100});
    CHECK(evaluation.knowledge_id == decay_object.knowledge_id);
    CHECK(!evaluation.is_expired);
    CHECK(evaluation.current_confidence < 0.8);
    CHECK(evaluation.should_revalidate == (evaluation.current_confidence < 0.5));

    if (g_failures == 0) {
        std::cout << "PASS: Phase 3 integration tests (20 checks)\n";
        return 0;
    }

    std::cerr << "FAILURES: " << g_failures << "\n";
    return 1;
}
