#pragma once

#include "ICandidateRegistry.h"

#include <array>
#include <cstdint>
#include <map>
#include <vector>

namespace xauusd::sovereign {

class CandidateRegistry final : public ICandidateRegistry {
public:
    CandidateRegistry() = default;

    bool add(const Candidate& candidate) override;
    bool contains(const EntityId& candidate_id) const override;
    bool get(const EntityId& candidate_id, Candidate& out) const override;
    bool update_status(const EntityId& candidate_id,
                       CandidateStatus new_status) override;
    std::vector<Candidate> all() const override;
    std::vector<Candidate> children_of(const EntityId& parent_id) const override;
    std::size_t size() const override;

    void clear();

private:
    std::vector<Candidate> items_;
    std::map<std::array<std::uint8_t, 16>, std::size_t> index_;
};

} // namespace xauusd::sovereign
