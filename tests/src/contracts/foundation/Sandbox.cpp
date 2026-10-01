#include "Sandbox.h"

namespace xauusd::sovereign {

bool Sandbox::prepare(const EntityId& experiment_id,
                      const SandboxConfig& config) {
    const auto key = experiment_id.bytes();
    if (index_.find(key) != index_.end()) {
        return false;
    }

    SandboxRun run(
        experiment_id,
        experiment_id,
        SandboxStatus::PREPARING,
        config,
        Timestamp{},
        Timestamp{},
        std::string{});

    const std::size_t position = runs_.size();
    runs_.push_back(std::move(run));
    index_.emplace(key, position);
    return true;
}

bool Sandbox::run(const EntityId& run_id) {
    const auto it = index_.find(run_id.bytes());
    if (it == index_.end()) {
        return false;
    }

    SandboxRun& run = runs_[it->second];
    if (run.status != SandboxStatus::PREPARING) {
        return false;
    }

    run.status = SandboxStatus::RUNNING;
    run.status = SandboxStatus::COMPLETED;
    return true;
}

bool Sandbox::abort(const EntityId& run_id) {
    const auto it = index_.find(run_id.bytes());
    if (it == index_.end()) {
        return false;
    }

    SandboxRun& run = runs_[it->second];
    if (run.status == SandboxStatus::COMPLETED ||
        run.status == SandboxStatus::FAILED ||
        run.status == SandboxStatus::ABORTED) {
        return false;
    }

    run.status = SandboxStatus::ABORTED;
    return true;
}

bool Sandbox::get_run(const EntityId& run_id, SandboxRun& out) const {
    const auto it = index_.find(run_id.bytes());
    if (it == index_.end()) {
        return false;
    }

    out = runs_[it->second];
    return true;
}

std::vector<SandboxRun> Sandbox::all_runs() const {
    return runs_;
}

std::size_t Sandbox::run_count() const {
    return runs_.size();
}

void Sandbox::clear() {
    runs_.clear();
    index_.clear();
}

} // namespace xauusd::sovereign
