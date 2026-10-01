#pragma once

#include "FailureMemoryEntry.h"

#include <vector>

namespace xauusd::sovereign {

class IFailureMemory {
public:
    virtual ~IFailureMemory() = default;

    virtual bool record(const FailureMemoryEntry& entry) = 0;
    virtual bool contains(const EntityId& entry_id) const = 0;
    virtual bool get(const EntityId& entry_id, FailureMemoryEntry& out_entry) const = 0;
    virtual std::vector<FailureMemoryEntry> all() const = 0;
    virtual std::size_t size() const = 0;
};

} // namespace xauusd::sovereign
