#pragma once

#include "KnowledgeStore.h"
#include "FailureMemory.h"

#include <vector>

namespace xauusd::ui {

struct LearningSourceStatus {
    bool knowledge_bound{false};
    bool failures_bound{false};
};

struct LearningSnapshot {
    std::vector<xauusd::sovereign::KnowledgeObject> knowledge;
    std::vector<xauusd::sovereign::FailureMemoryEntry> failures;
};

class LearningAdapter {
public:
    LearningAdapter(
        const xauusd::sovereign::KnowledgeStore* knowledge,
        const xauusd::sovereign::FailureMemory* failures) noexcept;

    LearningSnapshot snapshot() const;
    bool has_sources() const noexcept;
    LearningSourceStatus sources() const noexcept;

    void bind(
        const xauusd::sovereign::KnowledgeStore* knowledge,
        const xauusd::sovereign::FailureMemory* failures) noexcept;

private:
    const xauusd::sovereign::KnowledgeStore* knowledge_{nullptr};
    const xauusd::sovereign::FailureMemory* failures_{nullptr};
};

} // namespace xauusd::ui
