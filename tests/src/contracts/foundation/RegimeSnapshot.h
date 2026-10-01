#pragma once
#include "EntityId.h"
#include "RegimeType.h"
#include "Timestamp.h"
#include "Timeframe.h"
#include <cstdint>
namespace xauusd::sovereign {struct RegimeSnapshot{EntityId snapshot_id{};Timeframe timeframe{};Timestamp computed_at{};RegimeType regime{RegimeType::UNKNOWN};double confidence{0.0};std::uint32_t persistence_bars{0};bool is_stable{false};};}
