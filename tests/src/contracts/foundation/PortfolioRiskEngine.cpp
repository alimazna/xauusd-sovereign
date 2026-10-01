#include "PortfolioRiskEngine.h"
#include <cmath>
namespace xauusd::sovereign {
bool PortfolioRiskEngine::approve(RiskProposal&p)const{if(!p.approved||!std::isfinite(p.risk_fraction)||p.risk_fraction<=0.0)return false;if(reservations_.find(p.proposal_id.bytes())!=reservations_.end()){p.approved=false;return false;}if(open_positions_>=max_positions_){p.approved=false;return false;}if(exposure_+p.risk_fraction>max_exposure_+1e-12){p.approved=false;return false;}if(daily_loss_fraction_>=max_daily_loss_fraction_-1e-12){p.approved=false;return false;}reservations_[p.proposal_id.bytes()]=p.risk_fraction;++open_positions_;exposure_+=p.risk_fraction;return true;}
bool PortfolioRiskEngine::release(const EntityId&proposal_id)const{const auto it=reservations_.find(proposal_id.bytes());if(it==reservations_.end())return false;exposure_=std::max(0.0,exposure_-it->second);if(open_positions_>0)--open_positions_;reservations_.erase(it);return true;}
}
