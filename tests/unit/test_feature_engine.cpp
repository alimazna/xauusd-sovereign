#include "TestHelpers.h"

#include "FeatureEngine.h"
int main(){
 FeatureEngine e; assert(e.feature_count()>=12);
 auto first=e.compute(Timeframe::M15,test_bar(1000,2000),nullptr);
 assert(std::holds_alternative<bool>(first.features.at(FeatureKey{"atr"}).storage()));
 for(int i=1;i<=15;++i)e.compute(Timeframe::M15,test_bar(1000+i*1000,2000.0+0.4*i),nullptr);
 auto f=e.compute(Timeframe::M15,test_bar(17000,2006.4),nullptr);
 assert(std::holds_alternative<double>(f.features.at(FeatureKey{"atr"}).storage()));
 assert(std::get<double>(f.features.at(FeatureKey{"atr"}).storage())>=0.0);
 return 0;
}
