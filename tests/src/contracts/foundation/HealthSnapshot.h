#pragma once

#include "EntityId.h"
#include "ServiceState.h"
#include "FreshnessState.h"
#include "Timestamp.h"
#include "Version.h"

namespace xauusd::sovereign
{

struct HealthSnapshot
{
    EntityId        service_id{};

    ServiceState    service_state{ServiceState::UNKNOWN};

    FreshnessState  freshness{FreshnessState::Unknown};

    Timestamp       observed{};

    Timestamp       policy{};

    Version         version{};

    HealthSnapshot() = default;

    // Legacy 3-arg constructor (freshness, observed, policy)
    HealthSnapshot(
        FreshnessState freshness_value,
        Timestamp      observed_value,
        Timestamp      policy_value)
        : freshness(freshness_value)
        , observed(observed_value)
        , policy(policy_value)
    {
    }

    // Full 5-arg constructor used by RuntimeEngine
    HealthSnapshot(
        EntityId       service_id_value,
        ServiceState   service_state_value,
        FreshnessState freshness_value,
        Timestamp      observed_value,
        Version        version_value)
        : service_id(service_id_value)
        , service_state(service_state_value)
        , freshness(freshness_value)
        , observed(observed_value)
        , version(version_value)
    {
    }
};

}
