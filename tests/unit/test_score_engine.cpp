#include "TestHelpers.h"

#include "ScoreEngine.h"
int main(){ScoreEngine e;auto f=numeric_features(2000,4,35,2,1998,1996,60,0.6);auto s=bullish_structure();RegimeSnapshot r{};r.regime=RegimeType::TREND_UP;r.confidence=.8;Signal sig{};sig.direction=SignalDirection::LONG;auto sc=e.compute(sig,r,s,f);assert(sc.value>=0&&sc.value<=100);return 0;}
