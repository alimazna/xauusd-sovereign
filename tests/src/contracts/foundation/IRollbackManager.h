#pragma once
#include "EntityId.h"
#include "RollbackRequest.h"
#include "RollbackResult.h"
#include <cstddef>
#include <vector>
namespace xauusd::sovereign {
class IRollbackManager {
public:
    virtual ~IRollbackManager() = default;
    virtual bool submit(const RollbackRequest& request) = 0;
    virtual bool record_result(const RollbackResult& result) = 0;
    virtual bool contains(const EntityId& request_id) const = 0;
    virtual bool get_request(const EntityId& request_id, RollbackRequest& out) const = 0;
    virtual std::vector<RollbackResult> all_results() const = 0;
    virtual std::size_t request_count() const = 0;
};
}
