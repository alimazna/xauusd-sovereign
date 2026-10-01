#pragma once

#include "HypothesisStore.h"
#include "ExperimentLedger.h"

#include <vector>

namespace xauusd::ui {

struct ResearchSourceStatus {
    bool hypotheses_bound{false};
    bool experiments_bound{false};
};

struct ResearchSnapshot {
    std::vector<xauusd::sovereign::Hypothesis> hypotheses;
    std::vector<xauusd::sovereign::Experiment> experiments;
};

class ResearchAdapter {
public:
    ResearchAdapter(
        const xauusd::sovereign::HypothesisStore* hypotheses,
        const xauusd::sovereign::ExperimentLedger* experiments) noexcept;

    ResearchSnapshot snapshot() const;
    bool has_sources() const noexcept;
    ResearchSourceStatus sources() const noexcept;

    void bind(
        const xauusd::sovereign::HypothesisStore* hypotheses,
        const xauusd::sovereign::ExperimentLedger* experiments) noexcept;

private:
    const xauusd::sovereign::HypothesisStore* hypotheses_{nullptr};
    const xauusd::sovereign::ExperimentLedger* experiments_{nullptr};
};

} // namespace xauusd::ui
