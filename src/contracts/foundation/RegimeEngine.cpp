#include "RegimeEngine.h"
#include "EngineIdentity.h"
#include <algorithm>
#include <cmath>
#include <limits>
#include <variant>
namespace xauusd::sovereign {
namespace{
int k(Timeframe t){return static_cast<int>(t);}
double getd(const FeatureSnapshot&s,const char*n){
 auto it=s.features.find(FeatureKey{n}); if(it==s.features.end())return std::numeric_limits<double>::quiet_NaN();
 if (auto p = std::get_if<double>(&it->second.storage())) {
   return *p;
 }
 return std::numeric_limits<double>::quiet_NaN();
}
double clamp(double x){return std::max(0.0,std::min(1.0,x));}
}
RegimeSnapshot RegimeEngine::classify(Timeframe tf,const FeatureSnapshot& f,const StructureSnapshot& s,const RegimeSnapshot* previous){
 double adx=getd(f,"adx"), slope=getd(f,"ema_slope"), natr=getd(f,"natr");
 RegimeType raw=RegimeType::UNKNOWN;
 if(std::isfinite(natr)&&natr<0.15) raw=RegimeType::COMPRESSION;
 else if(std::isfinite(adx)&&adx<20.0&&s.last_swing_type==StructureType::RANGE) raw=RegimeType::RANGE;
 else if(std::isfinite(adx)&&adx>25.0&&s.is_bullish&&slope>0){
   raw=(std::isfinite(natr)&&natr>1.0)?RegimeType::EXPANSION_UP:RegimeType::TREND_UP;
 } else if(std::isfinite(adx)&&adx>25.0&&!s.is_bullish&&slope<0){
   raw=(std::isfinite(natr)&&natr>1.0)?RegimeType::EXPANSION_DOWN:RegimeType::TREND_DOWN;
 }
 auto& p=pending_[k(tf)];
 RegimeType accepted=previous?previous->regime:RegimeType::UNKNOWN;
 std::uint32_t persistence=previous?previous->persistence_bars:0;
 bool stable=true;
 if(accepted==RegimeType::UNKNOWN){accepted=raw; p={}; stable=(raw!=RegimeType::UNKNOWN); persistence=stable?1:0;}
 else if(raw!=RegimeType::UNKNOWN && raw!=accepted){
   if(p.candidate==raw)++p.count; else {p.candidate=raw;p.count=1;}
   if(p.count>=2){accepted=raw;p={};p.count=0;stable=true;persistence=1;}
   else {stable=false; ++persistence;}
 } else {
   p={}; ++persistence;
 }
 double conf=std::isfinite(adx)?clamp(adx/50.0):0.0;
 if(s.has_valid_structure)conf=clamp(0.7*conf+0.3);
 RegimeSnapshot out;
 out.snapshot_id=detail::make_id("regime",static_cast<std::uint64_t>(tf),static_cast<std::uint64_t>(f.computed_at.value()),static_cast<std::uint64_t>(persistence));
 out.timeframe=tf;
 out.regime=(stable || accepted==RegimeType::UNKNOWN) ? accepted : RegimeType::TRANSITION;
 out.confidence=conf; out.persistence_bars=persistence; out.is_stable=stable;
 return out;
}
void RegimeEngine::clear(){pending_.clear();}
}
