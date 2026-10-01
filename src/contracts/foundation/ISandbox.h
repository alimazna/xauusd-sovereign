#pragma once

#include "EntityId.h"
#include "SandboxConfig.h"
#include "SandboxRun.h"

#include <cstddef>
#include <vector>

namespace xauusd::sovereign {

class ISandbox {
public:
    virtual ~ISandbox() = default;

    virtual bool prepare(const EntityId& experiment_id,
                         const SandboxConfig& config) = 0;
    virtual bool run(const EntityId& run_id) = 0;
    virtual bool abort(const EntityId& run_id) = 0;
    virtual bool get_run(const EntityId& run_id, SandboxRun& out) const = 0;
    virtual std::vector<SandboxRun> all_runs() const = 0;
    virtual std::size_t run_count() const = 0;
};

} // namespace xauusd::sovereign
