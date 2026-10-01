#include "HealthAggregator.h"
#include "Clock.h"
#include <utility>
namespace xauusd::sovereign {
void HealthAggregator::observe_snapshot(const HealthSnapshot&snapshot){snapshots_[snapshot.service_id.bytes()]=snapshot;}
void HealthAggregator::observe_failure(const FailurePattern&pattern){failures_[pattern.pattern_id.bytes()]=pattern;}
SystemHealthSnapshot HealthAggregator::current_health()const{std::uint32_t healthy_services=0,degraded_services=0,failed_services=0;bool has_offline=false,has_error=false;for(const auto&entry:snapshots_){const auto&snapshot=entry.second;switch(snapshot.service_state){case ServiceState::ONLINE:++healthy_services;break;case ServiceState::DEGRADED:++degraded_services;break;case ServiceState::OFFLINE:++failed_services;has_offline=true;break;case ServiceState::ERROR:++failed_services;has_error=true;break;default:break;}}SystemHealthLevel level=SystemHealthLevel::UNKNOWN;if(!snapshots_.empty()){if(has_error)level=SystemHealthLevel::FAILED;else if(has_offline)level=SystemHealthLevel::CRITICAL;else if(degraded_services>0)level=SystemHealthLevel::DEGRADED;else level=SystemHealthLevel::HEALTHY;}return SystemHealthSnapshot(detail::now_timestamp(),level,healthy_services,degraded_services,failed_services,{});}
HealthAggregate HealthAggregator::aggregate()const{std::vector<HealthSnapshot>services;services.reserve(snapshots_.size());for(const auto&entry:snapshots_)services.push_back(entry.second);std::vector<FailurePattern>recent_failures;recent_failures.reserve(failures_.size());for(const auto&entry:failures_)recent_failures.push_back(entry.second);const auto snapshot=current_health();return HealthAggregate(snapshot.taken_at,snapshot.level,std::move(services),std::move(recent_failures),{});}
void HealthAggregator::clear(){snapshots_.clear();failures_.clear();}
} // namespace xauusd::sovereign
