#pragma once

#include "EntityId.h"
#include "Timestamp.h"
#include "Timeframe.h"
#include "DataQualityState.h"
#include "StructureType.h"

namespace xauusd::sovereign
{

struct StructureSnapshot
{
    EntityId snapshot_id{};

    Timeframe timeframe{Timeframe::M1};

    Timestamp computed_at{};

    DataQualityState data_quality{
        DataQualityState::UNKNOWN_VALUE
    };

    StructureType last_swing_type{
        StructureType::NONE
    };

    double last_swing_high{0.0};

    double last_swing_low{0.0};

    double channel_upper{0.0};

    double channel_lower{0.0};

    bool has_valid_structure{false};

    bool is_bullish{false};
};

}