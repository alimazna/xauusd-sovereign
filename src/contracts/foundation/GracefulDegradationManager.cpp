#include "GracefulDegradationManager.h"
#include "Clock.h"
#include <algorithm>
namespace xauusd::sovereign {
GracefulDegradationManager::GracefulDegradationManager(const IDependencyGraph*graph,const ICapabilityRegistry*registry)noexcept:graph_(graph),registry_(registry){}
std::vector<DegradationImpact> GracefulDegradationManager::evaluate_impact(CapabilityId failed_capability)const{std::vector<DegradationImpact>result;if(graph_==nullptr||registry_==nullptr)return result;std::set<CapabilityId>visited;std::vector<std::pair<CapabilityId,ImpactLevel>>stack;visited.insert(failed_capability);for(const auto id:graph_->direct_dependents_of(failed_capability))stack.emplace_back(id,ImpactLevel::FULL);while(!stack.empty()){auto[current,level]=stack.back();stack.pop_back();if(!visited.insert(current).second)continue;result.push_back(DegradationImpact{current,level,"dependent on failed capability",detail::now_timestamp()});for(const auto dependent:graph_->direct_dependents_of(current))stack.emplace_back(dependent,ImpactLevel::PARTIAL);}std::sort(result.begin(),result.end(),[](const auto&a,const auto&b){return a.capability_id<b.capability_id;});return result;}
bool GracefulDegradationManager::is_capability_available(CapabilityId capability_id)const{
 if(registry_==nullptr)return available_.find(capability_id)!=available_.end()&&unavailable_.find(capability_id)==unavailable_.end();
 if(!registry_->contains(capability_id))return false;
 return available_.find(capability_id)!=available_.end()&&unavailable_.find(capability_id)==unavailable_.end();
}
void GracefulDegradationManager::mark_available(CapabilityId capability_id){if(registry_!=nullptr&&!registry_->contains(capability_id))return;available_.insert(capability_id);unavailable_.erase(capability_id);}
void GracefulDegradationManager::mark_unavailable(CapabilityId capability_id){unavailable_.insert(capability_id);available_.erase(capability_id);}
}
