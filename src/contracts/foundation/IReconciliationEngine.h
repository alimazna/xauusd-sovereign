#pragma once
#include "Position.h"
#include "ReconciliationResult.h"
#include "RiskProposal.h"
#include "Signal.h"
namespace xauusd::sovereign {class IReconciliationEngine{public:virtual~IReconciliationEngine()=default;virtual ReconciliationResult reconcile(const Signal&,const Position&) const=0;virtual ReconciliationResult reconcile(const Signal& signal,const RiskProposal&,const Position& position) const{return reconcile(signal,position);}};}
