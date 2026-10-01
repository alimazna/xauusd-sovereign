#pragma once
#include "IMarketQualityEngine.h"
#include <deque>
namespace xauusd::sovereign {class MarketQualityEngine final:public IMarketQualityEngine{public:MarketQuality evaluate(const Tick&) override;void clear();private:std::deque<double> spreads_;};}
