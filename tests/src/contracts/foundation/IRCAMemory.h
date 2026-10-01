#pragma once

#include "RCAHypothesis.h"

#include <vector>

namespace xauusd::sovereign {

class IRCAMemory {
public:
    virtual ~IRCAMemory() = default;

    virtual bool add_hypothesis(const RCAHypothesis& h) = 0;
    virtual bool confirm_hypothesis(const EntityId& hypothesis_id) = 0;
    virtual bool reject_hypothesis(const EntityId& hypothesis_id) = 0;
    virtual std::vector<RCAHypothesis> hypotheses_for(const EntityId& failure_id) const = 0;
};

} // namespace xauusd::sovereign
