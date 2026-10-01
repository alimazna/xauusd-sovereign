#pragma once

#include "IKnowledgeStore.h"

#include <array>
#include <cstdint>
#include <map>
#include <vector>

namespace xauusd::sovereign {

class KnowledgeStore final : public IKnowledgeStore {
public:
    KnowledgeStore() = default;

    bool add(const KnowledgeObject& object) override;
    bool contains(const KnowledgeId& id) const override;
    bool get(const KnowledgeId& id, KnowledgeObject& out_object) const override;
    bool update(const KnowledgeObject& object) override;
    std::vector<KnowledgeObject> all() const override;
    std::size_t size() const override;

    void clear();

private:
    std::vector<KnowledgeObject> objects_;
    std::map<std::array<std::uint8_t, 16>, std::size_t> index_;
};

} // namespace xauusd::sovereign
