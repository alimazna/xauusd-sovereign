#include "ReconciliationEngine.h"
#include "EngineIdentity.h"
#include <cmath>
namespace xauusd::sovereign {
ReconciliationResult ReconciliationEngine::reconcile(const Signal&s,const Position&p)const{
 ReconciliationResult r;r.decision_id=s.decision_id;r.reconciled_at=p.closed_at.value()!=0?p.closed_at:s.triggered_at;
 if(s.direction==SignalDirection::NONE||p.direction!=s.direction){r.reason="direction mismatch";return r;}
 if(detail::is_zero(p.signal_id)||p.signal_id!=s.signal_id){r.reason="signal identity mismatch";return r;}
 if(detail::is_zero(p.proposal_id)||detail::is_zero(p.fill_id)||detail::is_zero(p.position_id)){r.reason="position identity chain incomplete";return r;}
 if(std::abs(p.requested_price-s.entry_reference)>tolerance_){r.reason="requested entry mismatch";return r;}
 if(std::abs(std::abs(p.entry_price-p.requested_price)-std::abs(p.slippage))>tolerance_){r.reason="execution slippage mismatch";return r;}
 if(std::abs(p.stop_loss-s.invalidating_price)>tolerance_){r.reason="stop mismatch";return r;}
 if(p.take_profit<=0.0){r.reason="missing take profit";return r;}
 if(s.direction==SignalDirection::LONG&&p.take_profit<s.entry_reference){r.reason="take profit geometry mismatch";return r;}
 if(s.direction==SignalDirection::SHORT&&p.take_profit>s.entry_reference){r.reason="take profit geometry mismatch";return r;}
 if(!std::isfinite(p.volume)||p.volume<=0.0){r.reason="invalid position volume";return r;}
 if(!p.is_open&&(p.closed_at.value()<p.opened_at.value()||p.exit_price<=0.0)){r.reason="invalid closed position state";return r;}
 r.matches=true;r.reason="signal and simulated position align";return r;
}
ReconciliationResult ReconciliationEngine::reconcile(const Signal&s,const RiskProposal&proposal,const Position&p)const{
 auto r=reconcile(s,p);if(!r.matches)return r;
 if(detail::is_zero(proposal.proposal_id)||proposal.proposal_id!=p.proposal_id){r.matches=false;r.reason="risk proposal identity mismatch";return r;}
 if(std::abs(p.take_profit-proposal.take_profit)>tolerance_){r.matches=false;r.reason="take profit mismatch";return r;}
 if(std::abs(p.stop_loss-proposal.stop_loss)>tolerance_){r.matches=false;r.reason="stop mismatch";return r;}
 if(std::abs(p.volume-proposal.volume)>std::max(tolerance_*0.01,1e-9)){r.matches=false;r.reason="volume mismatch";return r;}
 if(std::abs(p.tick_size-proposal.tick_size)>1e-12||std::abs(p.tick_value-proposal.tick_value)>1e-12){r.matches=false;r.reason="monetary specification mismatch";return r;}
 return r;
}
}
