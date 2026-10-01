#pragma once

#include "FailurePattern.h"
#include "FailureReport.h"

#include <vector>

namespace xauusd::sovereign {

class IFailureDetector {
public:
    virtual ~IFailureDetector() = default;

    virtual bool observe(const FailurePattern& pattern) = 0;

    virtual std::vector<FailurePattern> all_patterns() const = 0;

    virtual FailureReport generate_report() const = 0;

    virtual std::size_t pattern_count() const = 0;
};

} // namespace xauusd::sovereign
