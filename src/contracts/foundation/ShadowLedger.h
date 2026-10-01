#pragma once

#include "IShadowLedger.h"

#include <cstddef>
#include <map>
#include <vector>

namespace xauusd::sovereign {

class ShadowLedger final : public IShadowLedger {
public:
    ShadowLedger() = default;

    bool append(const ShadowLedgerEntry& entry) override;

    bool contains(const EntityId& entry_id) const override;

    bool get(
        const EntityId& entry_id,
        ShadowLedgerEntry& out_entry) const override;

    std::vector<ShadowLedgerEntry> all() const override;

    std::size_t size() const override;

    void clear();

private:
    std::vector<ShadowLedgerEntry>                      order_;
    std::map<std::array<std::uint8_t, 16>, std::size_t> index_;
};

} // namespace xauusd::sovereign
