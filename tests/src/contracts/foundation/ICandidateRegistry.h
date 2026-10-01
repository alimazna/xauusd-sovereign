#pragma once

#include "EntityId.h"
#include "Candidate.h"

#include <cstddef>
#include <vector>

namespace xauusd::sovereign {

class ICandidateRegistry {
public:
    virtual ~ICandidateRegistry() = default;

    virtual bool add(const Candidate& candidate) = 0;
    virtual bool contains(const EntityId& candidate_id) const = 0;
    virtual bool get(const EntityId& candidate_id, Candidate& out) const = 0;
    virtual bool update_status(const EntityId& candidate_id,
                               CandidateStatus new_status) = 0;
    virtual std::vector<Candidate> all() const = 0;
    virtual std::vector<Candidate> children_of(const EntityId& parent_id) const = 0;
    virtual std::size_t size() const = 0;
};

} // namespace xauusd::sovereign
