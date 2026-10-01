#pragma once

#include "EntityId.h"
#include "ValidationProtocol.h"
#include "ValidationResult.h"
#include "ValidationRun.h"

#include <cstddef>
#include <vector>

namespace xauusd::sovereign {

class IValidationFirewall {
public:
    virtual ~IValidationFirewall() = default;

    virtual bool register_protocol(const ValidationProtocol& protocol) = 0;
    virtual bool get_protocol(const EntityId& protocol_id,
                              ValidationProtocol& out) const = 0;

    virtual bool record_run(const ValidationRun& run) = 0;
    virtual bool record_result(const ValidationResult& result) = 0;

    virtual bool all_passed(const EntityId& run_id) const = 0;
    virtual std::vector<ValidationResult> results_for(const EntityId& run_id) const = 0;
    virtual std::size_t protocol_count() const = 0;
    virtual std::size_t run_count() const = 0;
};

} // namespace xauusd::sovereign
