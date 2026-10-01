#pragma once
#include "IReconciliationEngine.h"
namespace xauusd::sovereign {class ReconciliationEngine final:public IReconciliationEngine{public:explicit ReconciliationEngine(double tolerance=0.02):tolerance_(tolerance){}ReconciliationResult reconcile(const Signal&,const Position&) const override;ReconciliationResult reconcile(const Signal&,const RiskProposal&,const Position&) const override;private:double tolerance_;};}
