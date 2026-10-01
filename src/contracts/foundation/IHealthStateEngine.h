#pragma once
#include "EntityId.h"
#include "ServiceState.h"
#include "FreshnessState.h"
#include "HealthSnapshot.h"
namespace xauusd::sovereign {
class IHealthStateEngine {
public:
    virtual ~IHealthStateEngine() = default;
    virtual HealthSnapshot current_snapshot(const EntityId&) const = 0;
    virtual bool record_state(const EntityId&, ServiceState, FreshnessState) = 0;
};
}
