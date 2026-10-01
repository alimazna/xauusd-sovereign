#pragma once

#include "EntityId.h"
#include "Experiment.h"
#include "ExperimentFingerprint.h"
#include "ExperimentResult.h"

#include <cstddef>
#include <vector>

namespace xauusd::sovereign {

class IExperimentLedger {
public:
    virtual ~IExperimentLedger() = default;

    virtual bool add(const Experiment& experiment) = 0;
    virtual bool record_result(const ExperimentResult& result) = 0;
    virtual bool contains(const EntityId& experiment_id) const = 0;
    virtual bool get(const EntityId& experiment_id, Experiment& out) const = 0;
    virtual bool has_fingerprint(const ExperimentFingerprint& fp) const = 0;
    virtual std::vector<Experiment> all() const = 0;
    virtual std::vector<ExperimentResult> results_for(const EntityId& experiment_id) const = 0;
    virtual std::size_t size() const = 0;
};

} // namespace xauusd::sovereign
