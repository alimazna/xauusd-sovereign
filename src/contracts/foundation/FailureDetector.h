#pragma once

#include "IFailureDetector.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <map>
#include <vector>

namespace xauusd::sovereign {

class FailureDetector final : public IFailureDetector {
public:
    FailureDetector() = default;

    bool observe(const FailurePattern& pattern) override;

    std::vector<FailurePattern> all_patterns() const override;

    FailureReport generate_report() const override;

    std::size_t pattern_count() const override;

    void clear();

private:
    std::vector<FailurePattern> patterns_;
    std::map<std::array<std::uint8_t, 16>, std::size_t> index_;
};

} // namespace xauusd::sovereign
