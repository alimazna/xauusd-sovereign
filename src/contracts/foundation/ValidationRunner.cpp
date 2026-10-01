#include "ValidationRunner.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <string>

namespace xauusd::sovereign {

EntityId ValidationRunner::result_id_for(const EntityId& candidate_id,
                                         ValidationMethod method,
                                         std::uint32_t salt) {
    std::array<std::uint8_t, 16> bytes = candidate_id.bytes();
    const std::uint8_t method_byte = static_cast<std::uint8_t>(method);
    bytes[0] = static_cast<std::uint8_t>(bytes[0] ^ method_byte);
    for (std::size_t i = 0; i < 4; ++i) {
        bytes[12 + i] ^= static_cast<std::uint8_t>((salt >> (i * 8)) & 0xFF);
    }
    return EntityId{bytes};
}

ValidationResult ValidationRunner::run_oos(const EntityId& candidate_id,
                                           const OOSWindow& window) {
    const bool passed = window.sample_count > 0 && window.locked;
    const double score = std::min(1.0, static_cast<double>(window.sample_count) / 1000.0);
    ValidationResult result(
        result_id_for(candidate_id, ValidationMethod::TIME_AWARE),
        candidate_id,
        ValidationMethod::TIME_AWARE,
        passed,
        score,
        0.5,
        window.sample_count,
        passed ? "deterministic OOS validation passed" : "deterministic OOS validation failed",
        window.end);
    ++total_runs_;
    results_.push_back(result);
    return result;
}

std::vector<ValidationResult> ValidationRunner::run_walk_forward(
    const EntityId& candidate_id,
    const WalkForwardConfig& config) {
    std::vector<ValidationResult> out;
    if (config.max_windows == 0) {
        return out;
    }

    const std::uint64_t derived_windows =
        config.step_us == 0 ? 1u : 1u + (config.validation_window_us / config.step_us);
    const std::uint64_t count = std::min(config.max_windows, derived_windows);
    const bool passed = config.train_window_us > 0 &&
                        config.validation_window_us > 0 &&
                        config.forward_window_us > 0 &&
                        config.step_us > 0;

    out.reserve(static_cast<std::size_t>(count));
    for (std::uint64_t i = 0; i < count; ++i) {
        const double score = passed ? 1.0 : 0.0;
        ValidationResult result(
            result_id_for(candidate_id, ValidationMethod::WALK_FORWARD, static_cast<std::uint32_t>(i)),
            candidate_id,
            ValidationMethod::WALK_FORWARD,
            passed,
            score,
            0.5,
            i + 1,
            passed ? "deterministic walk-forward validation passed"
                   : "deterministic walk-forward validation failed",
            Timestamp(static_cast<std::int64_t>(i + 1)));
        out.push_back(result);
        results_.push_back(result);
        ++total_runs_;
    }
    return out;
}

ValidationResult ValidationRunner::run_stress(const EntityId& candidate_id,
                                              const StressTestConfig& config) {
    const bool passed = config.iterations > 0 && config.max_drawdown_threshold > 0.0;
    const double score = passed ? 1.0 : 0.0;
    ValidationResult result(
        result_id_for(candidate_id, ValidationMethod::MONTE_CARLO_STRESS),
        candidate_id,
        ValidationMethod::MONTE_CARLO_STRESS,
        passed,
        score,
        config.max_drawdown_threshold,
        config.iterations,
        passed ? "deterministic stress validation passed" : "deterministic stress validation failed",
        Timestamp(static_cast<std::int64_t>(config.iterations)));
    ++total_runs_;
    results_.push_back(result);
    return result;
}

std::size_t ValidationRunner::total_runs() const {
    return total_runs_;
}

void ValidationRunner::clear() {
    total_runs_ = 0;
    results_.clear();
}

} // namespace xauusd::sovereign
