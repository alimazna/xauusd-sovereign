#include "TestHelpers.h"

#include "StructureEngine.h"
int main(){StructureEngine e;for(int i=0;i<24;++i){double c=2000+((i%6==2)?3.0:0.4*i);e.analyze(Timeframe::M15,test_bar(1000+i*1000,c));}auto s=e.analyze(Timeframe::M15,test_bar(25000,2010));assert(s.has_valid_structure);assert(s.channel_upper>=s.channel_lower);return 0;}
