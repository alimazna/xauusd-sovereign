#pragma once
#include "AccountState.h"
#include "MarketQuality.h"
#include "RiskLimits.h"
#include "RiskProposal.h"
#include "Signal.h"
#include "SymbolSpec.h"
namespace xauusd::sovereign {class IRiskEngine{public:virtual~IRiskEngine()=default;virtual RiskProposal propose(const Signal&,const AccountState&,const RiskLimits&,const SymbolSpec&,MarketQuality) const=0;};}
