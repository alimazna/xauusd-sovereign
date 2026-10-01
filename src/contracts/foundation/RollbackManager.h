#pragma once
#include "IRollbackManager.h"
#include <array>
#include <cstdint>
#include <map>
#include <vector>
namespace xauusd::sovereign {
class RollbackManager final : public IRollbackManager {
public:
    RollbackManager() = default;
    bool submit(const RollbackRequest& request) override;
    bool record_result(const RollbackResult& result) override;
    bool contains(const EntityId& request_id) const override;
    bool get_request(const EntityId& request_id, RollbackRequest& out) const override;
    std::vector<RollbackResult> all_results() const override;
    std::size_t request_count() const override;
    void clear();
private:
    std::vector<RollbackRequest> requests_;
    std::map<std::array<std::uint8_t, 16>, std::size_t> index_;
    std::vector<RollbackResult> results_;
};
}
