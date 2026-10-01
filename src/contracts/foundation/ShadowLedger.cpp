#include "ShadowLedger.h"

namespace xauusd::sovereign {

bool ShadowLedger::append(const ShadowLedgerEntry& entry)
{
    const auto key = entry.entry_id.bytes();
    if (index_.find(key) != index_.end()) {
        return false;
    }

    const std::size_t position = order_.size();
    order_.push_back(entry);
    index_.emplace(key, position);
    return true;
}

bool ShadowLedger::contains(const EntityId& entry_id) const
{
    return index_.find(entry_id.bytes()) != index_.end();
}

bool ShadowLedger::get(
    const EntityId& entry_id,
    ShadowLedgerEntry& out_entry) const
{
    const auto it = index_.find(entry_id.bytes());
    if (it == index_.end()) {
        return false;
    }

    out_entry = order_[it->second];
    return true;
}

std::vector<ShadowLedgerEntry> ShadowLedger::all() const
{
    return order_;
}

std::size_t ShadowLedger::size() const
{
    return order_.size();
}

void ShadowLedger::clear()
{
    order_.clear();
    index_.clear();
}

} // namespace xauusd::sovereign
