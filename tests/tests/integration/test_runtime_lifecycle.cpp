#include "TestHelpers.h"
#include "BarFinalizer.h"
#include "DataValidator.h"
#include "RuntimeEngine.h"
#include "EngineIdentity.h"
#include "TimeframeStateStore.h"
#include "ShadowLedger.h"
#include "AuditEngine.h"
#include "HealthMonitor.h"
#include "Watchdog.h"
#include "MarketQualityEngine.h"
#include "RiskEngine.h"
#include "PortfolioRiskEngine.h"
#include "ShadowExecutionEngine.h"
#include "PositionSimulator.h"
#include "ReconciliationEngine.h"
#include "ScoreEngine.h"
#include "ConfidenceEngine.h"
#include "MacroContextEngine.h"
#include <cassert>
#include <variant>
using namespace xauusd::sovereign;

class StubFeature final : public IFeatureEngine {
public:
    FeatureSnapshot compute(Timeframe tf,const Bar& bar,const FeatureSnapshot*) const override {
        FeatureSnapshot f; f.timeframe=tf; f.computed_at=bar.close_time; f.data_quality=DataQualityState::VALID_QUALITY;
        auto add=[&](const char*k,double v){f.features.emplace(FeatureKey{k},FeatureValue{v});};
        add("close",bar.close); add("atr",10.0); add("adx",40.0); add("ema_fast",2000.0); add("ema_slow",1990.0); add("ema_slope",1.0); add("rsi",60.0); add("natr",0.5);
        return f;
    }
    std::size_t feature_count() const override{return 8;}
};
class StubStructure final : public IStructureEngine {
public:
    StructureSnapshot analyze(Timeframe tf,const Bar& bar) override {StructureSnapshot s;s.timeframe=tf;s.computed_at=bar.close_time;s.data_quality=DataQualityState::VALID_QUALITY;s.last_swing_low=1990;s.last_swing_high=2010;s.last_swing_type=StructureType::HIGHER_LOW;s.has_valid_structure=true;s.is_bullish=true;return s;}
};
class StubRegime final : public IRegimeEngine {
public:
    RegimeSnapshot classify(Timeframe tf,const FeatureSnapshot& f,const StructureSnapshot&,const RegimeSnapshot*) override {RegimeSnapshot r;r.timeframe=tf;r.computed_at=f.computed_at;r.regime=RegimeType::TREND_UP;r.confidence=0.9;r.persistence_bars=1;r.is_stable=true;return r;}
};
class StubEligibility final : public IEligibilityEngine {
public:
    EligibilityResult evaluate(const RegimeSnapshot& r,const StructureSnapshot&,const FeatureSnapshot& f) const override {EligibilityResult e;e.computed_at=f.computed_at;if(r.regime==RegimeType::TREND_UP)e.eligible_families={StrategyFamily::TREND_CONTINUATION};return e;}
};
class StubSignal final : public ISignalEngine {
public:
    Signal generate(const EligibilityResult&,const RegimeSnapshot&,const StructureSnapshot&,const FeatureSnapshot& f) override {Signal s;s.triggered_at=f.computed_at;s.trigger_timeframe=f.timeframe;s.direction=SignalDirection::LONG;s.strategy=StrategyFamily::TREND_CONTINUATION;s.entry_reference=2000;s.invalidating_price=1990;s.rationale="deterministic runtime lifecycle test";s.decision_id=detail::make_id("runtime_decision",static_cast<std::uint64_t>(f.computed_at.value()),static_cast<std::uint64_t>(f.timeframe),1);s.signal_id=detail::make_id("runtime_signal",static_cast<std::uint64_t>(f.computed_at.value()),static_cast<std::uint64_t>(f.timeframe),static_cast<std::uint64_t>(s.direction),1);return s;}
};

int main(){
    DataValidator dv; BarFinalizer fin(&dv); TimeframeStateStore store; ShadowLedger ledger; StubFeature fe; StubStructure st; StubRegime re; StubEligibility el; StubSignal si; ScoreEngine score; ConfidenceEngine confidence; MacroContextEngine macro; MarketQualityEngine quality; RiskEngine risk; PortfolioRiskEngine portfolio(.10,1); ShadowExecutionEngine execution; PositionSimulator positions; ReconciliationEngine reconciliation; AuditEngine audit; HealthMonitor health; Watchdog watchdog;
    RuntimeEngine rt(&fin,&store,&ledger,&fe,&st,&re,&el,&si,&score,&confidence,&macro,&quality,&risk,&portfolio,&execution,&positions,&reconciliation,&audit,&health,&watchdog);
    SymbolSpec ss=RiskEngine::default_xauusd_spec(); rt.set_symbol_spec(ss);
    auto bar=test_bar(1000,2000); rt.ingest_tick(test_tick(900,2000,.2));
    auto first=rt.process_bar(Timeframe::M15,bar); assert(first.completed); assert(first.fill.state==FillState::FILLED); assert(first.position.is_open); assert(first.position.stop_loss==1990&&first.position.take_profit==2015); assert(rt.open_position_count()==1); assert(std::abs(first.risk.volume-.1)<1e-12); assert(std::abs(first.risk.risk_amount-100.0)<1e-9);
    auto duplicate=rt.process_bar(Timeframe::M15,bar); assert(!duplicate.completed); assert(rt.open_position_count()==1);
    rt.ingest_tick(Tick{Timestamp{1100},Timestamp{1110},2016.0,2016.1,2016.05,1.0,0});
    assert(rt.open_position_count()==0); assert(rt.closed_positions().size()==1); assert(rt.closed_positions().front().close_reason=="TP"); assert(rt.closed_positions().front().closed_at.value()==1100); assert(rt.account_state().balance>10000.0); assert(rt.account_state().daily_pnl>0.0); assert(rt.account_state().peak_equity==rt.account_state().equity); assert(rt.closed_reconciliations().size()==1); assert(rt.closed_reconciliations().front().matches); assert(portfolio.open_positions()==0&&std::abs(portfolio.exposure())<1e-12);
    auto second=rt.process_bar(Timeframe::M15,test_bar(2000,2000)); assert(second.completed); assert(rt.open_position_count()==1); rt.ingest_tick(Tick{Timestamp{2100},Timestamp{2110},1989.0,1989.1,1989.05,1.0,0}); assert(rt.open_position_count()==0); assert(rt.closed_positions().size()==2); assert(portfolio.open_positions()==0&&std::abs(portfolio.exposure())<1e-12);
    return 0;
}
