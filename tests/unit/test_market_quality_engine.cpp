#include "TestHelpers.h"

#include "MarketQualityEngine.h"
int main(){MarketQualityEngine e;for(int i=0;i<8;++i)e.evaluate(test_tick(1000+i*100,2000,.2));auto q=e.evaluate(test_tick(2000,2000,0.05));assert(q==MarketQuality::GOOD||q==MarketQuality::ACCEPTABLE);auto bad=e.evaluate(Tick{Timestamp{1},Timestamp{10001},2000,1999,1999.5,1,0});assert(bad==MarketQuality::POOR);return 0;}
