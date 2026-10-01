#pragma once

#include "IFailureMemory.h"
#include "IRCAMemory.h"

#include <array>
#include <cstdint>
#include <map>
#include <vector>

namespace xauusd::sovereign {

class FailureMemory final : public IFailureMemory, public IRCAMemory {
public:
    FailureMemory() = default;

    // IFailureMemory
    bool record(const FailureMemoryEntry& entry) override;
    bool contains(const EntityId& entry_id) const override;
    bool get(const EntityId& entry_id, FailureMemoryEntry& out_entry) const override;
    std::vector<FailureMemoryEntry> all() const override;
    std::size_t size() const override;

    // IRCAMemory
    bool add_hypothesis(const RCAHypothesis& h) override;
    bool confirm_hypothesis(const EntityId& hypothesis_id) override;
    bool reject_hypothesis(const EntityId& hypothesis_id) override;
    std::vector<RCAHypothesis> hypotheses_for(const EntityId& failure_id) const override;

    void clear();

private:
    std::vector<FailureMemoryEntry> entries_;
    std::map<std::array<std::uint8_t, 16>, std::size_t> entries_index_;
    std::vector<RCAHypothesis> hypotheses_;
    std::map<std::array<std::uint8_t, 16>, std::size_t> hypotheses_index_;
};

} // namespace xauusd::sovereign
