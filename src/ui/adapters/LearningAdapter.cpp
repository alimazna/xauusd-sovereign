#include "LearningAdapter.h"

namespace xauusd::ui {

LearningAdapter::LearningAdapter(
    const xauusd::sovereign::KnowledgeStore* knowledge,
    const xauusd::sovereign::FailureMemory* failures) noexcept
    : knowledge_(knowledge), failures_(failures) {}

LearningSnapshot LearningAdapter::snapshot() const {
    LearningSnapshot result;
    if (knowledge_ != nullptr) {
        result.knowledge = knowledge_->all();
    }
    if (failures_ != nullptr) {
        result.failures = failures_->all();
    }
    return result;
}

bool LearningAdapter::has_sources() const noexcept {
    return knowledge_ != nullptr || failures_ != nullptr;
}

LearningSourceStatus LearningAdapter::sources() const noexcept {
    return LearningSourceStatus{
        knowledge_ != nullptr,
        failures_ != nullptr
    };
}

void LearningAdapter::bind(
    const xauusd::sovereign::KnowledgeStore* knowledge,
    const xauusd::sovereign::FailureMemory* failures) noexcept {
    knowledge_ = knowledge;
    failures_ = failures;
}

} // namespace xauusd::ui
