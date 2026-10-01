#pragma once
#include "MarketQuality.h"
#include "Tick.h"
namespace xauusd::sovereign {class IMarketQualityEngine{public:virtual~IMarketQualityEngine()=default;virtual MarketQuality evaluate(const Tick& tick)=0;};}
