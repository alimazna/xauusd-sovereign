#pragma once

#include "EntityId.h"
#include "Hypothesis.h"

#include <cstddef>
#include <vector>

namespace xauusd::sovereign {

class IHypothesisStore {
public:
    virtual ~IHypothesisStore() = default;

    virtual bool add(const Hypothesis& hypothesis) = 0;
    virtual bool contains(const EntityId& hypothesis_id) const = 0;
    virtual bool get(const EntityId& hypothesis_id, Hypothesis& out) const = 0;
    virtual bool update_status(const EntityId& hypothesis_id,
                               HypothesisStatus new_status) = 0;
    virtual std::vector<Hypothesis> all() const = 0;
    virtual std::size_t size() const = 0;
};

} // namespace xauusd::sovereign
