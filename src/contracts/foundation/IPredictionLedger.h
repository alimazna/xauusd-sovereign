#pragma once

#include "EntityId.h"
#include "PredictionRecord.h"

#include <cstddef>
#include <vector>

namespace xauusd::sovereign {

class IPredictionLedger {
public:
    virtual ~IPredictionLedger() = default;

    virtual bool append(const PredictionRecord& record) = 0;

    virtual bool contains(const EntityId& record_id) const = 0;

    virtual bool get(
        const EntityId& record_id,
        PredictionRecord& out_record) const = 0;

    virtual std::vector<PredictionRecord> all() const = 0;

    virtual std::size_t size() const = 0;
};

} // namespace xauusd::sovereign
