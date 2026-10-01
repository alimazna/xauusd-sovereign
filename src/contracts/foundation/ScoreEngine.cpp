#include "ScoreEngine.h"
#include <algorithm>
#include <cmath>
#include <limits>
#include <variant>
namespace xauusd::sovereign {
namespace{double getd(const FeatureSnapshot&s,const char*n){auto it=s.features.find(FeatureKey{n});if(it==s.features.end())return std::numeric_limits<double>::quiet_NaN();if(auto p=std::get_if<double>(&it->second.storage()))return *p;return std::numeric_limits<double>::quiet_NaN();}double clamp(double v,double lo,double hi){return std::max(lo,std::min(hi,v));}}
Score ScoreEngine::compute(const Signal&sig,const RegimeSnapshot&r,const StructureSnapshot&s,const FeatureSnapshot&f) const{
 if(sig.direction==SignalDirection::NONE)return Score{0,0,100};
 double adx=getd(f,"adx"),slope=std::abs(getd(f,"ema_slope")),rsi=getd(f,"rsi"),natr=getd(f,"natr");
 const double rq=clamp(25.0*(r.confidence),0,25);
 const double sq=s.has_valid_structure?25.0:0.0;
 const double mq=std::isfinite(rsi)&&std::isfinite(slope)?clamp(25.0*(std::min(50.0,slope)/(std::max(1e-9,std::abs(getd(f,"close")))*0.005))*0.6+25.0*(1.0-std::abs(50.0-rsi)/50.0)*0.4,0,25):0;
 const double vq=std::isfinite(natr)?clamp(15.0*(1.0-std::min(1.0,std::abs(natr-0.5)/1.0)),0,15):0;
 const double con=(r.regime!=RegimeType::UNKNOWN && s.has_valid_structure && adx>25)?10.0:5.0;
 return Score{clamp(rq+sq+mq+vq+con,0,100),0,100};
}
}
