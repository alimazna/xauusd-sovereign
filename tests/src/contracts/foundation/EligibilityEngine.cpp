#include "EligibilityEngine.h"
namespace xauusd::sovereign {
EligibilityResult EligibilityEngine::evaluate(const RegimeSnapshot& r,const StructureSnapshot&,const FeatureSnapshot& f) const{
 EligibilityResult out; out.computed_at=f.computed_at;
 switch(r.regime){
 case RegimeType::TREND_UP: case RegimeType::TREND_DOWN:
  out.eligible_families={StrategyFamily::TREND_CONTINUATION,StrategyFamily::TREND_PULLBACK,StrategyFamily::MOMENTUM}; out.reason="trend regime"; break;
 case RegimeType::RANGE:
  out.eligible_families={StrategyFamily::MEAN_REVERSION,StrategyFamily::RANGE_BOUNDARY}; out.reason="range regime"; break;
 case RegimeType::COMPRESSION: out.eligible_families={StrategyFamily::BREAKOUT}; out.reason="compression regime"; break;
 case RegimeType::EXPANSION_UP: case RegimeType::EXPANSION_DOWN:
  out.eligible_families={StrategyFamily::MOMENTUM}; out.reason="expansion regime"; break;
 case RegimeType::POST_SHOCK: out.eligible_families={StrategyFamily::POST_SHOCK_REACTION}; out.reason="post-shock regime"; break;
 case RegimeType::TRANSITION: case RegimeType::UNKNOWN: default: out.reason="regime not eligible";
 }
 return out;
}
}
