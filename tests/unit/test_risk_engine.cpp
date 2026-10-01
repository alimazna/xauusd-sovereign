#include "TestHelpers.h"

#include "RiskEngine.h"
int main(){RiskEngine e;Signal s{};s.signal_id=test_id(1);s.triggered_at=Timestamp{1000};s.direction=SignalDirection::LONG;s.entry_reference=2000;s.invalidating_price=1990;AccountState a{};RiskLimits l{};auto p=e.propose(s,a,l,MarketQuality::GOOD);assert(p.approved);assert(p.volume>0);assert(p.take_profit>p.entry_price);return 0;}
