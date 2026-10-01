#include "TestHelpers.h"
#include "ShadowExecutionEngine.h"
int main(){ShadowExecutionEngine e(.01,3);RiskProposal p{};p.approved=true;p.signal_id=test_id(1);p.proposal_id=test_id(2);p.direction=SignalDirection::LONG;p.volume=1;p.entry_price=2000;p.stop_loss=1990;p.take_profit=2015;p.tick_size=.01;p.tick_value=1;p.proposed_at=Timestamp{100};auto f=e.execute(p,1999.9,2000.1,MarketQuality::GOOD);assert(f.state==FillState::FILLED);assert(f.fill_price>2000);assert(f.stop_loss==1990&&f.take_profit==2015);assert(f.tick_size==.01&&f.tick_value==1);assert(f.commission==3);return 0;}
