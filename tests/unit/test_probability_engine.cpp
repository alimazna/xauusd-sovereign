#include "TestHelpers.h"

#include "ProbabilityEngine.h"
int main(){ProbabilityEngine e;Score s{80,0,100};auto p=e.estimate(s);assert(!p.calibrated);assert(p.value<0);return 0;}
