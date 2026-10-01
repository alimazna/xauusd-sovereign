#pragma once

#include "EntityId.h"
#include "EvaluatorIdentity.h"

#include <cstddef>
#include <vector>

namespace xauusd::sovereign {

class IEvaluatorFirewall {
public:
    virtual ~IEvaluatorFirewall() = default;

    virtual bool register_evaluator(const EvaluatorIdentity& identity) = 0;
    virtual bool contains_evaluator(const EntityId& evaluator_id) const = 0;
    virtual bool get_evaluator(const EntityId& evaluator_id,
                               EvaluatorIdentity& out) const = 0;

    virtual bool freeze(const EntityId& evaluator_id) = 0;
    virtual bool is_frozen(const EntityId& evaluator_id) const = 0;

    virtual std::size_t evaluator_count() const = 0;
};

} // namespace xauusd::sovereign
