#pragma once
#include "EntityId.h"
#include "ServiceState.h"
#include "FreshnessState.h"
#include "HealthSnapshot.h"
#include "IHealthStateEngine.h"
#include <array>
#include <cstdint>
#include <map>
namespace xauusd::sovereign {
class HealthStateEngine final : public IHealthStateEngine {
public:
    HealthStateEngine() = default;
    HealthSnapshot current_snapshot(const EntityId& service_id) const override;
    bool record_state(const EntityId& service_id, ServiceState service_state, FreshnessState freshness) override;
    std::size_t tracked_services() const noexcept;
private:
    std::map<std::array<std::uint8_t, 16>, HealthSnapshot> snapshots_;
};
}
