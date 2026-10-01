#include "RiskEngine.h"
#include "EngineIdentity.h"
#include "PositionSizer.h"
#include <algorithm>
#include <cmath>
#include <limits>
namespace xauusd::sovereign {
namespace {
bool finite_positive(double v){return std::isfinite(v)&&v>0.0;}
bool valid_limits(const RiskLimits& l){
 return std::isfinite(l.max_risk_per_trade)&&std::isfinite(l.max_daily_loss)&&std::isfinite(l.max_exposure)&&
        std::isfinite(l.max_drawdown)&&std::isfinite(l.min_reward_to_risk)&&
        l.max_risk_per_trade>0.0&&l.max_risk_per_trade<=1.0&&l.max_daily_loss>0.0&&l.max_daily_loss<=1.0&&
        l.max_exposure>0.0&&l.max_exposure<=1.0&&l.max_drawdown>0.0&&l.max_drawdown<=1.0&&
        l.min_reward_to_risk>0.0&&l.max_risk_per_trade<=l.max_exposure;
}
bool valid_account(const AccountState&a){return std::isfinite(a.equity)&&std::isfinite(a.balance)&&std::isfinite(a.daily_pnl)&&std::isfinite(a.peak_equity)&&a.equity>0.0&&a.balance>0.0&&a.peak_equity>0.0;}
bool valid_spec(const SymbolSpec&s){
 return !s.symbol.empty()&&std::isfinite(s.point)&&std::isfinite(s.tick_size)&&std::isfinite(s.tick_value)&&std::isfinite(s.contract_size)&&
        std::isfinite(s.volume_min)&&std::isfinite(s.volume_max)&&std::isfinite(s.volume_step)&&
        s.point>0.0&&s.tick_size>0.0&&s.tick_value>0.0&&s.contract_size>0.0&&s.volume_min>0.0&&s.volume_max>=s.volume_min&&s.volume_step>0.0&&
        std::isfinite(s.stops_level)&&std::isfinite(s.freeze_level)&&s.stops_level>=0.0&&s.freeze_level>=0.0;
}
}
RiskProposal RiskEngine::propose(const Signal&s,const AccountState&a,const RiskLimits&l,const SymbolSpec&spec,MarketQuality q) const{
 RiskProposal p; p.signal_id=s.signal_id; p.proposed_at=s.triggered_at; p.direction=s.direction; p.entry_price=s.entry_reference;
 p.proposal_id=detail::derive_id("risk",s.signal_id,static_cast<std::uint64_t>(s.triggered_at.value()),static_cast<std::uint64_t>(s.direction));
 if(s.direction==SignalDirection::NONE||!finite_positive(s.entry_reference)||!finite_positive(s.invalidating_price))return p;
 if(!valid_account(a)||!valid_limits(l)||!valid_spec(spec))return p;
 if(q==MarketQuality::POOR||q==MarketQuality::UNKNOWN)return p;
 if((s.direction==SignalDirection::LONG && s.invalidating_price>=s.entry_reference) ||
    (s.direction==SignalDirection::SHORT && s.invalidating_price<=s.entry_reference)) return p;
 const double dist=std::abs(s.entry_reference-s.invalidating_price);
 const double min_stop=spec.stops_level*spec.point;
 if(!std::isfinite(dist)||dist<=0.0||dist<min_stop)return p;
 const double tp_dist=dist*l.min_reward_to_risk;
 if(!std::isfinite(tp_dist)||tp_dist<=0.0)return p;
 p.stop_loss=s.invalidating_price;
 p.take_profit=(s.direction==SignalDirection::LONG)?s.entry_reference+tp_dist:s.entry_reference-tp_dist;
 p.tick_size=spec.tick_size; p.tick_value=spec.tick_value;
 const double requested_risk=a.equity*l.max_risk_per_trade;
 PositionSizer sizer;
 const PositionSizing sized=sizer.size(requested_risk,dist,spec);
 if(!sized.valid)return p;
 double volume=sized.volume;
 const double drawdown=std::max(0.0,(a.peak_equity-a.equity)/a.peak_equity);
 if(drawdown>l.max_drawdown){p.approved=false;return p;}
 if(drawdown>0.5*l.max_drawdown)volume*=0.5;
 if(q==MarketQuality::DEGRADED)volume*=0.5;
 volume=std::floor((volume/spec.volume_step)+1e-12)*spec.volume_step;
 if(!std::isfinite(volume)||volume<spec.volume_min||volume>spec.volume_max)return p;
 p.volume=volume;
 p.risk_amount=(dist/spec.tick_size)*spec.tick_value*p.volume;
 p.risk_fraction=p.risk_amount/a.equity;
 if(!std::isfinite(p.risk_amount)||!std::isfinite(p.risk_fraction)||p.risk_amount<=0.0||p.risk_fraction<=0.0||p.risk_fraction>l.max_risk_per_trade+1e-12)return p;
 if(a.daily_pnl<=-a.balance*l.max_daily_loss)return p;
 p.approved=true;
 return p;
}
}
