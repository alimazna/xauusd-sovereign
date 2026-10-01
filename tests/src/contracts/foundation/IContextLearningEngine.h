#pragma once

#include "ContextLearningRecord.h"

#include <vector>

namespace xauusd::sovereign {

class IContextLearningEngine {
public:
    virtual ~IContextLearningEngine() = default;

    virtual bool learn(const ContextLearningRecord& record) = 0;
    virtual std::vector<ContextLearningRecord> query(const ContextKey& key) const = 0;
    virtual std::size_t size() const = 0;
};

} // namespace xauusd::sovereign
