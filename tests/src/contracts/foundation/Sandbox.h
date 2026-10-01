#pragma once

#include "ISandbox.h"

#include <array>
#include <cstdint>
#include <map>
#include <vector>

namespace xauusd::sovereign {

class Sandbox final : public ISandbox {
public:
    Sandbox() = default;

    bool prepare(const EntityId& experiment_id,
                 const SandboxConfig& config) override;
    bool run(const EntityId& run_id) override;
    bool abort(const EntityId& run_id) override;
    bool get_run(const EntityId& run_id, SandboxRun& out) const override;
    std::vector<SandboxRun> all_runs() const override;
    std::size_t run_count() const override;

    void clear();

private:
    std::vector<SandboxRun> runs_;
    std::map<std::array<std::uint8_t, 16>, std::size_t> index_;
};

} // namespace xauusd::sovereign
