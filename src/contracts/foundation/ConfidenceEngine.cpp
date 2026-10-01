#include "ConfidenceEngine.h"
#include <algorithm>
#include <cmath>
#include <variant>
namespace xauusd::sovereign {
namespace{bool present(const FeatureSnapshot&s,const char*n){auto it=s.features.find(FeatureKey{n});return it!=s.features.end()&&std::holds_alternative<double>(it->second.storage());}}
Confidence ConfidenceEngine::compute(const Signal&sig,const RegimeSnapshot&r,const StructureSnapshot&s,const FeatureSnapshot&f) const{
 if(sig.direction==SignalDirection::NONE)return Confidence{};
 double completeness=0; const char* names[]={"atr","adx","ema_fast","ema_slow","ema_slope","rsi","natr"}; for(auto*n:names)if(present(f,n))completeness+=1.0/7.0;
 double timeframe_agreement=s.has_valid_structure?1.0:0.0;
 double regime_certainty=r.is_stable?1.0:r.confidence*0.75;
 double structure_clarity=s.has_valid_structure?1.0:0.0;
 double n=100.0*(0.30*completeness+0.25*timeframe_agreement+0.25*regime_certainty+0.20*structure_clarity);
 ConfidenceLevel l=n<30?ConfidenceLevel::NONE:n<=50?ConfidenceLevel::LOW:n<=75?ConfidenceLevel::MEDIUM:ConfidenceLevel::HIGH;
 return Confidence{l,n};
}
}
