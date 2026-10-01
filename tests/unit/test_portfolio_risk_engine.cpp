#include "TestHelpers.h"

#include "PortfolioRiskEngine.h"
int main(){PortfolioRiskEngine e(.02,1);RiskProposal p{};p.approved=true;p.risk_fraction=.01;assert(e.approve(p));RiskProposal q{};q.approved=true;q.risk_fraction=.01;assert(!e.approve(q));return 0;}
