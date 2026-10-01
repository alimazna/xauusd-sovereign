#include "TestHelpers.h"

#include "EligibilityEngine.h"
int main(){EligibilityEngine e;auto f=numeric_features(2000,4,35,2,1998,1996,62,0.6);auto s=bullish_structure();RegimeSnapshot r{};r.regime=RegimeType::TREND_UP;r.is_stable=true;r.confidence=0.8;auto x=e.evaluate(r,s,f);assert(x.eligible_families.size()==3);return 0;}
