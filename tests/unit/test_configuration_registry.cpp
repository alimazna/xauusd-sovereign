#include "TestHelpers.h"

#include "ConfigurationRegistry.h"
int main(){ConfigurationRegistry c;auto a=c.snapshot();assert(a.version.value()==0);c.set(ConfigurationKey{"risk"},ConfigurationValue{1.0});ConfigurationValue out;assert(c.get(ConfigurationKey{"risk"},out));assert(c.snapshot().version.value()==1);return 0;}
