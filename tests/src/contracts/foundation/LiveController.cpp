#include "LiveController.h"
#include "Clock.h"
#include "EngineIdentity.h"
#include <cmath>
#include <utility>
namespace xauusd::sovereign {
bool LiveController::enable(const LiveConfig&config){
 if(config.max_risk_per_trade_bps==0||config.max_daily_loss_bps==0||config.max_total_exposure_bps==0||config.max_concurrent_positions==0||config.broker_symbol.empty()||config.account_currency.empty())return false;
 if(config.max_risk_per_trade_bps>10000||config.max_daily_loss_bps>10000||config.max_total_exposure_bps>10000)return false;
 enabled_=true;status_=LiveModeStatus::MANUAL_ONLY;config_=config;session_=LiveSession(detail::make_id("live_session",detail::now_timestamp().value(),config.max_concurrent_positions),status_,config_,detail::now_timestamp(),Timestamp{},0,0);return true;
}
bool LiveController::disable(){enabled_=false;status_=LiveModeStatus::DISABLED;session_.status=status_;session_.ended_at=detail::now_timestamp();return true;}
bool LiveController::is_enabled()const{return enabled_;}
LiveModeStatus LiveController::current_status()const{return status_;}
bool LiveController::record_decision(const LiveDecision&decision){if(!enabled_||detail::is_zero(decision.decision_id)||decision.decided_at.value()<=0)return false;decisions_.push_back(decision);if(decision.accepted)++session_.trades_executed;else ++session_.trades_rejected;return true;}
std::vector<LiveDecision> LiveController::all_decisions()const{return decisions_;}
LiveSession LiveController::current_session()const{return session_;}
std::size_t LiveController::decision_count()const{return decisions_.size();}
void LiveController::clear(){enabled_=false;status_=LiveModeStatus::DISABLED;config_=LiveConfig{};session_=LiveSession{};decisions_.clear();}
} // namespace xauusd::sovereign
