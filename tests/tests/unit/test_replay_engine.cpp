#include "TestHelpers.h"

#include "ReplayEngine.h"
int main(){ReplayEngine e;std::vector<Bar> bars{test_bar(1000,2000),test_bar(2000,2001)};e.set_data(Timeframe::M15,bars);assert(e.load(Timeframe::M15));assert(e.has_next());assert(e.next().close==2000);assert(e.next().close==2001);assert(!e.has_next());e.reset();assert(e.has_next());return 0;}
