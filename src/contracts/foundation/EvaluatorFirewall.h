#pragma once

#include "IEvaluatorFirewall.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <map>
#include <set>
#include <vector>

namespace xauusd::sovereign {

class EvaluatorFirewall final : public IEvaluatorFirewall {
public:
    EvaluatorFirewall() = default;

    bool register_evaluator(const EvaluatorIdentity& identity) override;
    bool contains_evaluator(const EntityId& evaluator_id) const override;
    bool get_evaluator(const EntityId& evaluator_id,
                       EvaluatorIdentity& out) const override;

    bool freeze(const EntityId& evaluator_id) override;
    bool is_frozen(const EntityId& evaluator_id) const override;

    std::size_t evaluator_count() const override;

    void clear();

private:
    using Key = std::array<std::uint8_t, 16>;

    static Key key_of(const EntityId& id);

    std::vector<EvaluatorIdentity> identities_;
    std::map<Key, std::size_t> index_;
    std::set<Key> frozen_;
};

} // namespace xauusd::sovereign
