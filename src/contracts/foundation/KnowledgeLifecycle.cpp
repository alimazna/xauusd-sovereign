#include "KnowledgeLifecycle.h"

namespace xauusd::sovereign {

bool KnowledgeLifecycle::transition(const KnowledgeTransition& t) {
    const auto source_bytes = t.knowledge_id.entity_id().bytes();
    auto entry_bytes = source_bytes;

    const std::size_t ordinal = history_.size() + 1U;
    for (std::size_t i = 0U; i < sizeof(ordinal) && i < entry_bytes.size(); ++i) {
        entry_bytes[entry_bytes.size() - 1U - i] =
            static_cast<std::uint8_t>(
                entry_bytes[entry_bytes.size() - 1U - i] ^
                static_cast<std::uint8_t>((ordinal >> (i * 8U)) & 0xFFU));
    }

    history_.emplace_back(
        EntityId{entry_bytes},
        t.knowledge_id,
        t,
        t.transition_time);

    return true;
}

std::vector<KnowledgeHistoryEntry> KnowledgeLifecycle::history_for(
    const KnowledgeId& id) const {
    std::vector<KnowledgeHistoryEntry> result;

    for (const auto& entry : history_) {
        if (entry.knowledge_id == id) {
            result.push_back(entry);
        }
    }

    return result;
}

bool KnowledgeLifecycle::register_contradiction(
    const KnowledgeContradiction& c) {
    for (const auto& existing : contradictions_) {
        if (existing.contradiction_id == c.contradiction_id) {
            return false;
        }
    }

    contradictions_.push_back(c);
    return true;
}

std::size_t KnowledgeLifecycle::transition_count() const {
    return history_.size();
}

void KnowledgeLifecycle::clear() {
    history_.clear();
    contradictions_.clear();
}

} // namespace xauusd::sovereign
