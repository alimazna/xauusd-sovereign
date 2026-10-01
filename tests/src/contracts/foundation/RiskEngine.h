#pragma once
#include "IRiskEngine.h"
namespace xauusd::sovereign {
class RiskEngine final:public IRiskEngine{
public:
 static SymbolSpec default_xauusd_spec(){return SymbolSpec{"Research","Shadow","XAUUSD",2,0.01,0.01,1.0,100.0,0.01,100.0,0.01,0.0,0.0};}
 RiskProposal propose(const Signal&,const AccountState&,const RiskLimits&,const SymbolSpec&,MarketQuality) const override;
 RiskProposal propose(const Signal&s,const AccountState&a,const RiskLimits&l,MarketQuality q) const { return propose(s,a,l,default_xauusd_spec(),q); }
};
}
