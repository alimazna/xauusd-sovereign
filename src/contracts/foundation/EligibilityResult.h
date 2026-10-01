#pragma once
#include "StrategyFamily.h"
#include "Timestamp.h"
#include <string>
#include <utility>
#include <vector>
namespace xauusd::sovereign {struct EligibilityResult{Timestamp computed_at{};std::vector<StrategyFamily> eligible_families;std::string reason;EligibilityResult()=default;};}
