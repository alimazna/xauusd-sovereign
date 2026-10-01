#include "TestHelpers.h"

#include "Watchdog.h"
int main(){Watchdog w;assert(w.observe(Timestamp{1},true,0,10,ServiceState::ONLINE,ServiceState::ONLINE));assert(!w.observe(Timestamp{2},false,0,10,ServiceState::ONLINE,ServiceState::ONLINE));return 0;}
