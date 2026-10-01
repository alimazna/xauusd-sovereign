#include "TestHelpers.h"

#include "SignalEngine.h"
int main(){SignalEngine e;auto f=numeric_features(2000,20,35,10,1990,1980,60,1.0);auto s=bullish_structure();s.last_swing_low=1985;RegimeSnapshot r{};r.regime=RegimeType::TREND_UP;r.is_stable=true;EligibilityResult er;er.eligible_families={StrategyFamily::TREND_CONTINUATION};auto sig=e.generate(er,r,s,f);assert(sig.direction==SignalDirection::LONG);assert(sig.entry_reference==2000);return 0;}
