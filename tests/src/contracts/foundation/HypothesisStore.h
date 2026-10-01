#pragma once

#include "IHypothesisStore.h"

#include <array>
#include <cstdint>
#include <map>
#include <vector>

namespace xauusd::sovereign {

class HypothesisStore final : public IHypothesisStore {
public:
    HypothesisStore() = default;

    bool add(const Hypothesis& hypothesis) override;
    bool contains(const EntityId& hypothesis_id) const override;
    bool get(const EntityId& hypothesis_id, Hypothesis& out) const override;
    bool update_status(const EntityId& hypothesis_id,
                       HypothesisStatus new_status) override;
    std::vector<Hypothesis> all() const override;
    std::size_t size() const override;

    void clear();

private:
    std::vector<Hypothesis> items_;
    std::map<std::array<std::uint8_t, 16>, std::size_t> index_;
};

} // namespace xauusd::sovereign
