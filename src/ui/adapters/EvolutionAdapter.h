#pragma once

#include "CandidateRegistry.h"

#include <vector>

namespace xauusd::ui {

class EvolutionAdapter {
public:
    explicit EvolutionAdapter(const xauusd::sovereign::CandidateRegistry* registry) noexcept;

    bool has_registry() const noexcept;
    std::vector<xauusd::sovereign::Candidate> snapshot() const;

    void bind(const xauusd::sovereign::CandidateRegistry* registry) noexcept;

private:
    const xauusd::sovereign::CandidateRegistry* registry_{nullptr};
};

} // namespace xauusd::ui
