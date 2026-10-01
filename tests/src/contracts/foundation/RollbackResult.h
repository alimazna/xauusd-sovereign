#pragma once
#include "EntityId.h"
#include "RollbackRequest.h"
#include "Timestamp.h"
#include <string>
#include <utility>
namespace xauusd::sovereign {
struct RollbackResult {
    EntityId        result_id;
    RollbackRequest request;
    bool            succeeded;
    std::string     notes;
    Timestamp       completed_at;
    RollbackResult() = default;
    RollbackResult(EntityId result_id, RollbackRequest request,
                   bool succeeded, std::string notes, Timestamp completed_at)
        : result_id(result_id), request(std::move(request)), succeeded(succeeded),
          notes(std::move(notes)), completed_at(completed_at) {}
};
}
