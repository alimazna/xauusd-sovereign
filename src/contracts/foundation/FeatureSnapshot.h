#pragma once

#include "EntityId.h"
#include "Timestamp.h"
#include "Timeframe.h"
#include "DataQualityState.h"
#include "FeatureKey.h"
#include "FeatureValue.h"
#include <map>

namespace xauusd::sovereign
{

struct FeatureSnapshot
{
    EntityId snapshot_id{};

    EntityId source_bar_id{};

    Timeframe timeframe{Timeframe::M1};

    Timestamp computed_at{};

    DataQualityState data_quality{
        DataQualityState::UNKNOWN_VALUE
    };

    double close_price{0.0};

    double return_value{0.0};

    double volatility{0.0};

    double moving_average_fast{0.0};

    double moving_average_slow{0.0};

    double momentum{0.0};

    bool has_features{false};

    bool is_valid{false};

    std::map<FeatureKey, FeatureValue> features{};
};

}
