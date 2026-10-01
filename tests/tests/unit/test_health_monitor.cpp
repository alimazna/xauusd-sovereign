#include "TestHelpers.h"

#include "HealthMonitor.h"
int main(){HealthMonitor h;auto id=test_id(10);h.record(HealthSnapshot{id,ServiceState::ONLINE,FreshnessState::FRESH,Timestamp{1},Version{1}});assert(h.overall_state()==ServiceState::ONLINE);h.record(HealthSnapshot{id,ServiceState::DEGRADED,FreshnessState::AGING,Timestamp{2},Version{1}});assert(h.overall_state()==ServiceState::DEGRADED);return 0;}
