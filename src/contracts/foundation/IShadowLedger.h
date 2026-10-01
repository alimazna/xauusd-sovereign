#pragma once

#include "EntityId.h"
#include "ShadowLedgerEntry.h"

#include <cstddef>
#include <vector>

namespace xauusd::sovereign {

class IShadowLedger {
public:
    virtual ~IShadowLedger() = default;

    virtual bool append(const ShadowLedgerEntry& entry) = 0;

    virtual bool contains(const EntityId& entry_id) const = 0;

    virtual bool get(
        const EntityId& entry_id,
        ShadowLedgerEntry& out_entry) const = 0;

    virtual std::vector<ShadowLedgerEntry> all() const = 0;

    virtual std::size_t size() const = 0;
};

} // namespace xauusd::sovereign
