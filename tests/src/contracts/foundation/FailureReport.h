#pragma once

#include "EntityId.h"
#include "Timestamp.h"
#include "FailurePattern.h"

#include <string>
#include <utility>
#include <vector>

namespace xauusd::sovereign {

class FailureReport {
public:
    EntityId                     report_id;
    Timestamp                    generated_at;
    std::vector<FailurePattern>  patterns;
    std::string                  summary;

    FailureReport() = default;

    FailureReport(
        EntityId report_id,
        Timestamp generated_at,
        std::vector<FailurePattern> patterns,
        std::string summary)
        : report_id(report_id),
          generated_at(generated_at),
          patterns(std::move(patterns)),
          summary(std::move(summary)) {}
};

} // namespace xauusd::sovereign
