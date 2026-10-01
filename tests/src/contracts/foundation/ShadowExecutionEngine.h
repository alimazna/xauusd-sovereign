#pragma once
#include "IShadowExecutionEngine.h"
namespace xauusd::sovereign {class ShadowExecutionEngine final:public IShadowExecutionEngine{public:explicit ShadowExecutionEngine(double fixed_buffer=0.01,double commission_per_lot=3.0):fixed_buffer_(fixed_buffer),commission_per_lot_(commission_per_lot){}SimulatedFill execute(const RiskProposal&,double,double,MarketQuality) override;private:double fixed_buffer_;double commission_per_lot_;};}
