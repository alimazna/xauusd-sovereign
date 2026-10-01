#include "ResearchAdapter.h"

namespace xauusd::ui {

ResearchAdapter::ResearchAdapter(
    const xauusd::sovereign::HypothesisStore* hypotheses,
    const xauusd::sovereign::ExperimentLedger* experiments) noexcept
    : hypotheses_(hypotheses), experiments_(experiments) {}

ResearchSnapshot ResearchAdapter::snapshot() const {
    ResearchSnapshot result;
    if (hypotheses_ != nullptr) {
        result.hypotheses = hypotheses_->all();
    }
    if (experiments_ != nullptr) {
        result.experiments = experiments_->all();
    }
    return result;
}

bool ResearchAdapter::has_sources() const noexcept {
    return hypotheses_ != nullptr || experiments_ != nullptr;
}

ResearchSourceStatus ResearchAdapter::sources() const noexcept {
    return ResearchSourceStatus{
        hypotheses_ != nullptr,
        experiments_ != nullptr
    };
}

void ResearchAdapter::bind(
    const xauusd::sovereign::HypothesisStore* hypotheses,
    const xauusd::sovereign::ExperimentLedger* experiments) noexcept {
    hypotheses_ = hypotheses;
    experiments_ = experiments;
}

} // namespace xauusd::ui
