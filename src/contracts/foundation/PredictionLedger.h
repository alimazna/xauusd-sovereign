#pragma once

#include "IPredictionLedger.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <map>
#include <vector>

namespace xauusd::sovereign {

class PredictionLedger final : public IPredictionLedger {
public:
    PredictionLedger() = default;

    bool append(const PredictionRecord& record) override;

    bool contains(const EntityId& record_id) const override;

    bool get(
        const EntityId& record_id,
        PredictionRecord& out_record) const override;

    std::vector<PredictionRecord> all() const override;

    std::size_t size() const override;

    void clear();

private:
    std::vector<PredictionRecord> records_;
    std::map<std::array<std::uint8_t, 16>, std::size_t> index_;
};

} // namespace xauusd::sovereign
