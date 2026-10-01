#include "TestHelpers.h"
#include "BarFinalizer.h"
#include "CanaryController.h"
#include "CapabilityRegistry.h"
#include "DataValidator.h"
#include "DependencyGraph.h"
#include "FeatureEngine.h"
#include "GracefulDegradationManager.h"
#include "MarketQualityEngine.h"
#include "PortfolioRiskEngine.h"
#include "PositionSimulator.h"
#include "ReconciliationEngine.h"
#include "ResourceGovernor.h"
#include "RiskEngine.h"
#include "ScheduleManager.h"
#include "ShadowExecutionEngine.h"
#include "TimeframeStateStore.h"
#include <cassert>
#include <cmath>
#include <limits>
#include <map>
#include <vector>
using namespace xauusd::sovereign;

static SymbolSpec spec(){SymbolSpec s{};s.broker="Test";s.server="Test";s.symbol="XAUUSD";s.digits=2;s.point=.01;s.tick_size=.01;s.tick_value=1.;s.contract_size=100.;s.volume_min=.01;s.volume_max=100.;s.volume_step=.01;s.observed_at=Timestamp{123};return s;}
static Signal long_signal(){Signal s{};s.signal_id=test_id(9);s.decision_id=test_id(10);s.triggered_at=Timestamp{1000};s.trigger_timeframe=Timeframe::M15;s.direction=SignalDirection::LONG;s.strategy=StrategyFamily::TREND_CONTINUATION;s.entry_reference=2000.;s.invalidating_price=1990.;return s;}
static RiskProposal proposal_from(const Signal&s){RiskEngine e;AccountState a{};RiskLimits l{};return e.propose(s,a,l,spec(),MarketQuality::GOOD);}

int main(){
    // 1: end-to-end SL/TP preservation + monetary PnL
    { auto s=long_signal(); auto p=proposal_from(s); assert(p.approved); ShadowExecutionEngine ex; auto f=ex.execute(p,1999.9,2000.1,MarketQuality::GOOD); assert(f.state==FillState::FILLED); assert(f.stop_loss==p.stop_loss&&f.take_profit==p.take_profit); PositionSimulator ps; auto pos=ps.open(f); assert(pos.stop_loss==p.stop_loss&&pos.take_profit==p.take_profit); assert(pos.signal_id==s.signal_id&&pos.proposal_id==p.proposal_id&&pos.fill_id==f.fill_id); auto t=Tick{Timestamp{1100},Timestamp{1110},2016.,2016.1,2016.05,1.,0}; assert(ps.update(pos,t)); assert(!pos.is_open); assert(pos.closed_at.value()==1100); assert(pos.close_reason=="TP"); assert(pos.gross_pnl>0.0); }
    // 2: market quality stable spread is GOOD
    {MarketQualityEngine e;for(int i=0;i<5;++i)assert(e.evaluate(Tick{Timestamp{100+i},Timestamp{110+i},2000.,2000.2,2000.1,1.,0})!=MarketQuality::POOR);auto q=e.evaluate(Tick{Timestamp{200},Timestamp{210},2000.,2000.2,2000.1,1.,0});assert(q==MarketQuality::GOOD);}
    // 3: sizing obeys SymbolSpec and risk metadata matches final volume
    {Signal s=long_signal();RiskLimits l{};l.max_risk_per_trade=.01;AccountState a{};auto p=RiskEngine{}.propose(s,a,l,spec(),MarketQuality::GOOD);assert(p.approved);assert(std::abs(p.volume-.1)<1e-12);assert(std::abs(p.risk_amount-100.)<1e-9);assert(std::abs(p.risk_fraction-.01)<1e-12);}
    // 4: invalid risk limits / SL geometry rejected
    {Signal s=long_signal();s.invalidating_price=2010;assert(!RiskEngine{}.propose(s,AccountState{},RiskLimits{},spec(),MarketQuality::GOOD).approved);RiskLimits bad{};bad.min_reward_to_risk=-2;assert(!RiskEngine{}.propose(long_signal(),AccountState{},bad,spec(),MarketQuality::GOOD).approved);bad=RiskLimits{};bad.max_risk_per_trade=std::numeric_limits<double>::quiet_NaN();assert(!RiskEngine{}.propose(long_signal(),AccountState{},bad,spec(),MarketQuality::GOOD).approved);}
    // 5: portfolio reservation is released
    {PortfolioRiskEngine e(.02,1);auto p=proposal_from(long_signal());assert(e.approve(p));assert(!e.approve(p));assert(e.release(p.proposal_id));RiskProposal q=proposal_from(long_signal());q.proposal_id=test_id(44);assert(e.approve(q));}
    // 6: out-of-order store/feature/structure cannot overwrite latest
    {TimeframeStateStore store;Bar a=test_bar(1000,2000);TimeframeState s1(Timeframe::M15,a,Timestamp{1000},DataQualityState::VALID_QUALITY,BarFinalizationState::FINALIZED);store.put_state(s1);Bar old=test_bar(900,1999);TimeframeState s2(Timeframe::M15,old,Timestamp{900},DataQualityState::VALID_QUALITY,BarFinalizationState::FINALIZED);store.put_state(s2);TimeframeState out{};assert(store.get_state(Timeframe::M15,out));assert(out.last_update_time.value()==1000);FeatureEngine fe;auto f1=fe.compute(Timeframe::M15,a,nullptr);auto f2=fe.compute(Timeframe::M15,old,&f1);assert(f2.data_quality==DataQualityState::OUT_OF_ORDER_QUALITY);assert(f1.computed_at==f2.computed_at);}
    // 7: timeframe mismatch rejected by finalizer
    {DataValidator dv;BarFinalizer fin(&dv);auto b=test_bar(1000,2000,Timeframe::M15);DataValidationResult r{};assert(fin.process_bar(Timeframe::H1,b,r)==BarFinalizationState::REJECTED);}
    // 8: validator timing/NaN guards
    {DataValidator dv;Tick t{Timestamp{200},Timestamp{100},2000,2000.1,2000.05,1,0};assert(dv.validate_tick(t).outcome==ValidationOutcome::REJECTED_VALIDATION);auto s=spec();s.tick_value=std::numeric_limits<double>::quiet_NaN();assert(dv.validate_symbol_spec(s).outcome==ValidationOutcome::REJECTED_VALIDATION);assert(dv.validate_symbol_spec(spec()).observed_at.value()==123);}
    // 9: reconciliation catches identity, TP and invalid closure state
    {ReconciliationEngine r(.5);auto s=long_signal();auto p=proposal_from(s);ShadowExecutionEngine ex;auto f=ex.execute(p,1999.9,2000.1,MarketQuality::GOOD);PositionSimulator ps;auto pos=ps.open(f);assert(r.reconcile(s,pos).matches);pos.take_profit=1900;assert(!r.reconcile(s,pos).matches);}
    // 10: canary only decides pending next-stage requests
    {CanaryController c;assert(c.configure(CanaryConfig(CanaryStage::STAGE_1_MINIMAL,100,200,true)));auto req=ScaleUpRequest(test_id(50),CanaryStage::STAGE_1_MINIMAL,CanaryStage::STAGE_2_SMALL,"promote",Timestamp{10});auto bad=ScaleUpDecision(test_id(51),req,true,"ok",Timestamp{11});assert(!c.decide_scale_up(bad));assert(c.submit_scale_up(req));assert(c.decide_scale_up(bad));assert(c.current_stage()==CanaryStage::STAGE_2_SMALL);}
    // 11: schedule gates active transitions by enabled window
    {ScheduleManager m;OperatingSchedule s({ScheduleWindow(Timestamp{1},Timestamp{10},5,3,8,true,true)},"UTC",3,8);assert(m.configure(s));assert(!m.transition_to(OperatingMode::ACTIVE,Timestamp{20},"out"));assert(m.transition_to(OperatingMode::ACTIVE,Timestamp{5},"in"));}
    // 12: unknown resource types fail closed
    {ResourceGovernor g;g.set_budget(ResourceBudget(std::map<ResourceType,std::uint64_t>{{ResourceType::CPU,10}},8,2,3));assert(!g.consume(ResourceType::NETWORK,1));assert(g.consume(ResourceType::CPU,10));assert(!g.consume(ResourceType::CPU,1));}
    // 13: capability availability fails closed until registered + marked available
    {CapabilityRegistry reg;DependencyGraph graph;GracefulDegradationManager g(&graph,&reg);assert(!g.is_capability_available(CapabilityId::M15_FEED));CapabilityDescriptor d(CapabilityId::M15_FEED,"M15",CriticalityLevel::CRITICAL,false,true);assert(reg.register_capability(d));assert(!g.is_capability_available(CapabilityId::M15_FEED));g.mark_available(CapabilityId::M15_FEED);assert(g.is_capability_available(CapabilityId::M15_FEED));}
    return 0;
}
