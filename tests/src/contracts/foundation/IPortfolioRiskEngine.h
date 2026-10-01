#pragma once
#include "RiskLimits.h"
#include "RiskProposal.h"
namespace xauusd::sovereign {class IPortfolioRiskEngine{public:virtual~IPortfolioRiskEngine()=default;virtual bool approve(RiskProposal&) const=0;virtual bool release(const EntityId& proposal_id) const=0;virtual void set_daily_loss_fraction(double value) noexcept=0;virtual void configure(const RiskLimits& limits) noexcept=0;};}
