#pragma once

#include "IContextLearningEngine.h"

#include <vector>

namespace xauusd::sovereign {

class ContextLearningEngine final : public IContextLearningEngine {
public:
    ContextLearningEngine() = default;

    bool learn(const ContextLearningRecord& record) override;
    std::vector<ContextLearningRecord> query(const ContextKey& key) const override;
    std::size_t size() const override;

    void clear();

private:
    std::vector<ContextLearningRecord> records_;
};

} // namespace xauusd::sovereign
