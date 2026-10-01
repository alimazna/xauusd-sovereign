#include "MarketQualityEngine.h"
#include <algorithm>
#include <cmath>
#include <vector>
namespace xauusd::sovereign {
MarketQuality MarketQualityEngine::evaluate(const Tick&tick){
 if(!std::isfinite(tick.bid)||!std::isfinite(tick.ask)||tick.bid<=0||tick.ask<tick.bid)return MarketQuality::POOR;
 if(tick.receive_time.value()<tick.event_time.value())return MarketQuality::POOR;
 const double spread=tick.ask-tick.bid; if(!std::isfinite(spread)||spread<0.0)return MarketQuality::POOR;
 spreads_.push_back(spread);if(spreads_.size()>32)spreads_.pop_front();
 if(spreads_.size()<5)return MarketQuality::ACCEPTABLE;
 std::vector<double> v(spreads_.begin(),spreads_.end());std::sort(v.begin(),v.end());double med=v[v.size()/2];
 if(med<=0.0)return spread<=0.0?MarketQuality::GOOD:MarketQuality::POOR;
 const double ratio=spread/med; const auto delay=std::llabs(tick.receive_time.value()-tick.event_time.value());
 if(delay>5000)return MarketQuality::POOR;
 if(ratio<=1.25)return MarketQuality::GOOD;
 if(ratio<=1.50)return MarketQuality::ACCEPTABLE;
 if(ratio<=2.00)return MarketQuality::DEGRADED;
 return MarketQuality::POOR;
}
void MarketQualityEngine::clear(){spreads_.clear();}
}
