#pragma once

#include "EntityId.h"
#include "OutcomeRecord.h"

#include <cstddef>
#include <vector>

namespace xauusd::sovereign {

class IOutcomeEngine {
public:
    virtual ~IOutcomeEngine() = default;

    virtual bool record(const OutcomeRecord& record) = 0;

    virtual bool contains(const EntityId& record_id) const = 0;

    virtual bool get(
        const EntityId& record_id,
        OutcomeRecord& out_record) const = 0;

    virtual std::vector<OutcomeRecord> all() const = 0;

    virtual std::size_t size() const = 0;
};

} // namespace xauusd::sovereign
