#include "EvolutionAdapter.h"

namespace xauusd::ui {

EvolutionAdapter::EvolutionAdapter(const xauusd::sovereign::CandidateRegistry* registry) noexcept
    : registry_(registry) {}

bool EvolutionAdapter::has_registry() const noexcept {
    return registry_ != nullptr;
}

std::vector<xauusd::sovereign::Candidate> EvolutionAdapter::snapshot() const {
    return registry_ == nullptr ? std::vector<xauusd::sovereign::Candidate>{} : registry_->all();
}

void EvolutionAdapter::bind(const xauusd::sovereign::CandidateRegistry* registry) noexcept {
    registry_ = registry;
}

} // namespace xauusd::ui
