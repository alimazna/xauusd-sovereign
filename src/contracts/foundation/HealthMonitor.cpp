#include "HealthMonitor.h"
namespace xauusd::sovereign {
void HealthMonitor::record(const HealthSnapshot&s){for(auto&x:snapshots_)if(x.service_id==s.service_id){x=s;return;}snapshots_.push_back(s);}
bool HealthMonitor::get(EntityId id,HealthSnapshot&out) const{for(const auto&x:snapshots_)if(x.service_id==id){out=x;return true;}return false;}
std::vector<HealthSnapshot> HealthMonitor::all() const{return snapshots_;}
ServiceState HealthMonitor::overall_state() const noexcept{
 bool degraded=false;
 for(const auto&x:snapshots_){if(x.service_state==ServiceState::ERROR||x.service_state==ServiceState::BLOCKED)return ServiceState::ERROR;if(x.service_state==ServiceState::OFFLINE||x.service_state==ServiceState::PAUSED)degraded=true;if(x.service_state==ServiceState::DEGRADED)degraded=true;}
 return snapshots_.empty()?ServiceState::STARTING:(degraded?ServiceState::DEGRADED:ServiceState::ONLINE);
}
std::size_t HealthMonitor::size() const noexcept{return snapshots_.size();}
void HealthMonitor::clear(){snapshots_.clear();}
}
