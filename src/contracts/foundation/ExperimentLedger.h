#pragma once

#include "IExperimentLedger.h"

#include <array>
#include <cstdint>
#include <map>
#include <vector>

namespace xauusd::sovereign {

class ExperimentLedger final : public IExperimentLedger {
public:
    ExperimentLedger() = default;

    bool add(const Experiment& experiment) override;
    bool record_result(const ExperimentResult& result) override;
    bool contains(const EntityId& experiment_id) const override;
    bool get(const EntityId& experiment_id, Experiment& out) const override;
    bool has_fingerprint(const ExperimentFingerprint& fp) const override;
    std::vector<Experiment> all() const override;
    std::vector<ExperimentResult> results_for(const EntityId& experiment_id) const override;
    std::size_t size() const override;

    void clear();

private:
    std::vector<Experiment> items_;
    std::map<std::array<std::uint8_t, 16>, std::size_t> index_;
    std::vector<ExperimentResult> results_;
};

} // namespace xauusd::sovereign
