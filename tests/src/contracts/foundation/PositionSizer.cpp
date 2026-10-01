#include "PositionSizer.h"
#include <algorithm>
#include <cmath>
namespace xauusd::sovereign {
PositionSizing PositionSizer::size(double risk_amount,double stop_distance,const SymbolSpec&s) const{
 PositionSizing out;if(risk_amount<=0||stop_distance<=0||s.tick_size<=0||s.tick_value<=0||s.volume_step<=0)return out;
 double v=risk_amount/(stop_distance*s.tick_value/s.tick_size);
 v=std::floor(v/s.volume_step)*s.volume_step;
 if(v<s.volume_min||v>s.volume_max)return out;
 out.volume=v;out.monetary_risk=v*stop_distance*s.tick_value/s.tick_size;out.valid=true;return out;
}
}
