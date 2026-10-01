#pragma once
#include "MarketQuality.h"
#include "RiskProposal.h"
#include "SimulatedFill.h"
namespace xauusd::sovereign {class IShadowExecutionEngine{public:virtual~IShadowExecutionEngine()=default;virtual SimulatedFill execute(const RiskProposal&,double current_bid,double current_ask,MarketQuality)=0;};}
