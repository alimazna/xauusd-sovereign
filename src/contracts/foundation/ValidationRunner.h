#pragma once

#include "IValidationRunner.h"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace xauusd::sovereign {

class ValidationRunner final : public IValidationRunner {
public:
    ValidationRunner() = default;

    ValidationResult run_oos(const EntityId& candidate_id,
                             const OOSWindow& window) override;

    std::vector<ValidationResult> run_walk_forward(
        const EntityId& candidate_id,
        const WalkForwardConfig& config) override;

    ValidationResult run_stress(const EntityId& candidate_id,
                                const StressTestConfig& config) override;

    std::size_t total_runs() const override;

    void clear();

private:
    static EntityId result_id_for(const EntityId& candidate_id,
                                  ValidationMethod method,
                                  std::uint32_t salt = 0);

    std::size_t total_runs_{0};
    std::vector<ValidationResult> results_;
};

} // namespace xauusd::sovereign
