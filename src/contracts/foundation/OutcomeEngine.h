#pragma once

#include "IOutcomeEngine.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <map>
#include <vector>

namespace xauusd::sovereign {

class OutcomeEngine final : public IOutcomeEngine {
public:
    OutcomeEngine() = default;

    bool record(const OutcomeRecord& record) override;

    bool contains(const EntityId& record_id) const override;

    bool get(
        const EntityId& record_id,
        OutcomeRecord& out_record) const override;

    std::vector<OutcomeRecord> all() const override;

    std::size_t size() const override;

    void clear();

private:
    std::vector<OutcomeRecord> records_;
    std::map<std::array<std::uint8_t, 16>, std::size_t> index_;
};

} // namespace xauusd::sovereign
