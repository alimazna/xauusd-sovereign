#pragma once
#include "IPortfolioRiskEngine.h"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <map>
namespace xauusd::sovereign {
class PortfolioRiskEngine final:public IPortfolioRiskEngine{
public:
 explicit PortfolioRiskEngine(double exposure=0.10,std::size_t max_positions=10):max_exposure_(exposure),max_positions_(max_positions){}
 bool approve(RiskProposal&) const override; bool release(const EntityId& proposal_id) const override;
 void set_daily_loss_fraction(double value) noexcept override{daily_loss_fraction_=std::max(0.0,value);}
 void configure(const RiskLimits& limits) noexcept override{max_exposure_=std::clamp(limits.max_exposure,0.0,1.0);max_daily_loss_fraction_=std::clamp(limits.max_daily_loss,0.0,1.0);}
 void set_state(double exposure,std::size_t open_positions,double daily_loss_fraction) noexcept{exposure_=std::max(0.0,exposure);open_positions_=open_positions;daily_loss_fraction_=std::max(0.0,daily_loss_fraction);reservations_.clear();}
 double exposure()const noexcept{return exposure_;}std::size_t open_positions()const noexcept{return open_positions_;}
private:double max_exposure_{0.10};std::size_t max_positions_{10};double max_daily_loss_fraction_{0.03};mutable double exposure_{0.0};mutable std::size_t open_positions_{0};mutable double daily_loss_fraction_{0.0};mutable std::map<std::array<std::uint8_t,16>,double> reservations_;
};
}
