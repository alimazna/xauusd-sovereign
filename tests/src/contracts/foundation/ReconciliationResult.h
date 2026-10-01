#pragma once
#include "EntityId.h"
#include "Timestamp.h"
#include <string>
namespace xauusd::sovereign {struct ReconciliationResult{EntityId decision_id{};Timestamp reconciled_at{};bool matches{false};std::string reason;};}
