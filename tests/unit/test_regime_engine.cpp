#include "TestHelpers.h"

#include "RegimeEngine.h"
int main(){RegimeEngine e;auto f=numeric_features(2000,4,35,2,1998,1996,62,0.6);auto s=bullish_structure();auto r=e.classify(Timeframe::M15,f,s,nullptr);assert(r.regime==RegimeType::TREND_UP||r.regime==RegimeType::UNKNOWN);auto r2=e.classify(Timeframe::M15,f,s,&r);assert(r2.confidence>=0);return 0;}
