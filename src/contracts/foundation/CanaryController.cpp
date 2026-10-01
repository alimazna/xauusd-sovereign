#include "CanaryController.h"
#include "EngineIdentity.h"
#include <algorithm>
namespace xauusd::sovereign {
namespace {bool is_next(CanaryStage from,CanaryStage to){return static_cast<int>(to)==static_cast<int>(from)+1;}}
bool CanaryController::configure(const CanaryConfig&config){if(config.max_drawdown_bps_before_rollback==0)return false;config_=config;pending_.clear();decisions_.clear();return true;}
CanaryStage CanaryController::current_stage()const{return config_.current_stage;}
bool CanaryController::submit_scale_up(const ScaleUpRequest&request){
 if(detail::is_zero(request.request_id)||request.requested_at.value()<=0||request.justification.empty())return false;
 if(request.from_stage!=config_.current_stage||!is_next(request.from_stage,request.to_stage))return false;
 const bool duplicate_pending=std::any_of(pending_.begin(),pending_.end(),[&](const auto&r){return r.request_id==request.request_id;});
 const bool duplicate_decided=std::any_of(decisions_.begin(),decisions_.end(),[&](const auto&d){return d.request.request_id==request.request_id;});
 if(duplicate_pending||duplicate_decided)return false;
 pending_.push_back(request);return true;
}
bool CanaryController::decide_scale_up(const ScaleUpDecision&decision){
 if(detail::is_zero(decision.decision_id)||decision.decided_at.value()<=0||decision.reason.empty())return false;
 if(std::any_of(decisions_.begin(),decisions_.end(),[&](const auto&d){return d.decision_id==decision.decision_id;}))return false;
 const auto it=std::find_if(pending_.begin(),pending_.end(),[&](const auto&r){return r.request_id==decision.request.request_id;});
 if(it==pending_.end())return false;
 if(decision.request.from_stage!=config_.current_stage||!is_next(decision.request.from_stage,decision.request.to_stage))return false;
 if(decision.decided_at.value()<it->requested_at.value())return false;
 if(decision.approved)config_.current_stage=decision.request.to_stage;
 decisions_.push_back(decision);pending_.erase(it);return true;
}
std::vector<ScaleUpDecision> CanaryController::all_decisions()const{return decisions_;}
void CanaryController::clear(){config_=CanaryConfig{};pending_.clear();decisions_.clear();}
}
