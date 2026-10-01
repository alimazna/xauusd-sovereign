#include "ContradictionEngine.h"

namespace xauusd::sovereign {

std::vector<KnowledgeContradiction> ContradictionEngine::detect_conflicts(
    const std::vector<KnowledgeObject>& objects) const {
    std::vector<KnowledgeContradiction> detected;

    for (std::size_t i = 0U; i < objects.size(); ++i) {
        for (std::size_t j = i + 1U; j < objects.size(); ++j) {
            const auto& a = objects[i];
            const auto& b = objects[j];

            if (a.validity_scope.empty() ||
                a.validity_scope != b.validity_scope ||
                a.conclusion.empty() ||
                b.conclusion.empty() ||
                a.conclusion == b.conclusion) {
                continue;
            }

            auto contradiction_bytes = a.knowledge_id.entity_id().bytes();
            const auto b_bytes = b.knowledge_id.entity_id().bytes();
            for (std::size_t k = 0U; k < contradiction_bytes.size(); ++k) {
                contradiction_bytes[k] =
                    static_cast<std::uint8_t>(
                        contradiction_bytes[k] ^ b_bytes[k]);
            }

            const auto detected_at =
                (a.last_supported_at.value() >= b.last_supported_at.value())
                    ? a.last_supported_at
                    : b.last_supported_at;

            detected.emplace_back(
                EntityId{contradiction_bytes},
                a.knowledge_id,
                b.knowledge_id,
                a.validity_scope,
                detected_at,
                false);
        }
    }

    contradictions_.insert(
        contradictions_.end(),
        detected.begin(),
        detected.end());

    return detected;
}

bool ContradictionEngine::resolve(const EntityId& contradiction_id) {
    for (auto& contradiction : contradictions_) {
        if (contradiction.contradiction_id == contradiction_id) {
            if (contradiction.resolved) {
                return false;
            }

            contradiction.resolved = true;
            return true;
        }
    }

    return false;
}

void ContradictionEngine::clear() {
    contradictions_.clear();
}

} // namespace xauusd::sovereign
