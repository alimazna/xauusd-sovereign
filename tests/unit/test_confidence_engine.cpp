#include "TestHelpers.h"

#include "ConfidenceEngine.h"
int main(){ConfidenceEngine e;auto f=numeric_features(2000,4,35,2,1998,1996,60,0.6);auto s=bullish_structure();RegimeSnapshot r{};r.confidence=.9;r.is_stable=true;Signal sig{};sig.direction=SignalDirection::LONG;auto c=e.compute(sig,r,s,f);assert(c.numeric>0);assert(c.numeric<=100);return 0;}
