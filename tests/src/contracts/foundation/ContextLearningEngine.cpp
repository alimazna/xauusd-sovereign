#include "ContextLearningEngine.h"

namespace xauusd::sovereign {

bool ContextLearningEngine::learn(const ContextLearningRecord& record) {
    records_.push_back(record);
    return true;
}

std::vector<ContextLearningRecord> ContextLearningEngine::query(
    const ContextKey& key) const {
    std::vector<ContextLearningRecord> result;

    for (const auto& record : records_) {
        if (record.key == key) {
            result.push_back(record);
        }
    }

    return result;
}

std::size_t ContextLearningEngine::size() const {
    return records_.size();
}

void ContextLearningEngine::clear() {
    records_.clear();
}

} // namespace xauusd::sovereign
