#pragma once

#include "KnowledgeObject.h"

#include <vector>

namespace xauusd::sovereign {

class IKnowledgeStore {
public:
    virtual ~IKnowledgeStore() = default;

    virtual bool add(const KnowledgeObject& object) = 0;
    virtual bool contains(const KnowledgeId& id) const = 0;
    virtual bool get(const KnowledgeId& id, KnowledgeObject& out_object) const = 0;
    virtual bool update(const KnowledgeObject& object) = 0;
    virtual std::vector<KnowledgeObject> all() const = 0;
    virtual std::size_t size() const = 0;
};

} // namespace xauusd::sovereign
