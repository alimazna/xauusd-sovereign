#include "EvidenceFirewall.h"

namespace xauusd::sovereign {

EvidenceFirewall::EvidenceFirewall(std::size_t holdout_budget)
    : holdout_budget_(holdout_budget) {}

bool EvidenceFirewall::register_layer(const EvidenceLayer& layer) {
    for (auto& existing : layers_) {
        if (existing.zone == layer.zone) {
            existing = layer;
            return true;
        }
    }
    layers_.push_back(layer);
    return true;
}

bool EvidenceFirewall::contains_layer(EvidenceZone zone) const {
    for (const auto& layer : layers_) {
        if (layer.zone == zone) {
            return true;
        }
    }
    return false;
}

bool EvidenceFirewall::get_layer(EvidenceZone zone, EvidenceLayer& out) const {
    for (const auto& layer : layers_) {
        if (layer.zone == zone) {
            out = layer;
            return true;
        }
    }
    return false;
}

bool EvidenceFirewall::request_holdout(const HoldoutQuery& query) {
    if (holdout_used_ >= holdout_budget_) {
        return false;
    }
    for (const auto& existing : queries_) {
        if (existing.query_id == query.query_id) {
            return false;
        }
    }
    HoldoutQuery stored = query;
    stored.approved = true;
    stored.consumed = false;
    queries_.push_back(std::move(stored));
    return true;
}

bool EvidenceFirewall::consume_holdout(const EntityId& query_id) {
    for (auto& query : queries_) {
        if (query.query_id == query_id) {
            if (query.consumed || !query.approved || holdout_used_ >= holdout_budget_) {
                return false;
            }
            query.consumed = true;
            ++holdout_used_;
            return true;
        }
    }
    return false;
}

std::size_t EvidenceFirewall::holdout_query_count() const {
    return queries_.size();
}

std::size_t EvidenceFirewall::holdout_budget_remaining() const {
    return holdout_budget_ - holdout_used_;
}

bool EvidenceFirewall::record_snapshot(const EvidenceSnapshot& snapshot) {
    snapshots_.push_back(snapshot);
    return true;
}

std::vector<EvidenceSnapshot> EvidenceFirewall::snapshots_for(
    const EntityId& candidate_id) const {
    std::vector<EvidenceSnapshot> out;
    for (const auto& snapshot : snapshots_) {
        if (snapshot.candidate_id == candidate_id) {
            out.push_back(snapshot);
        }
    }
    return out;
}

void EvidenceFirewall::clear() {
    holdout_used_ = 0;
    layers_.clear();
    queries_.clear();
    snapshots_.clear();
}

} // namespace xauusd::sovereign
