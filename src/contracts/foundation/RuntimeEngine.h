#pragma once
#include "RuntimeState.h"
#include "ShadowDecision.h"
#include "ShadowLedger.h"
#include "IBarFinalizer.h"
#include "ITimeframeStateStore.h"
#include "IDataBus.h"
#include "DataValidationResult.h"
#include "IFeatureEngine.h"
#include "IStructureEngine.h"
#include "IRegimeEngine.h"
#include "IEligibilityEngine.h"
#include "ISignalEngine.h"
#include "IScoreEngine.h"
#include "IConfidenceEngine.h"
#include "IMacroContextEngine.h"
#include "IMarketQualityEngine.h"
#include "IRiskEngine.h"
#include "IPortfolioRiskEngine.h"
#include "IShadowExecutionEngine.h"
#include "IPositionSimulator.h"
#include "IReconciliationEngine.h"
#include "IAuditEngine.h"
#include "HealthMonitor.h"
#include "Watchdog.h"
#include "DecisionFlowResult.h"
#include "Tick.h"
#include "Timestamp.h"
#include "Timeframe.h"
#include "SymbolSpec.h"
#include "AccountState.h"
#include "RiskLimits.h"
#include <array>
#include <map>
#include <vector>
namespace xauusd::sovereign {
class RuntimeEngine {
public:
    RuntimeEngine(IBarFinalizer*,ITimeframeStateStore*,ShadowLedger*) noexcept;
    RuntimeEngine(IBarFinalizer*,ITimeframeStateStore*,ShadowLedger*,
                  IFeatureEngine*,IStructureEngine*,IRegimeEngine*,IEligibilityEngine*,
                  ISignalEngine*,IScoreEngine*,IConfidenceEngine*,IMacroContextEngine*,
                  IMarketQualityEngine*,IRiskEngine*,IPortfolioRiskEngine*,
                  IShadowExecutionEngine*,IPositionSimulator*,IReconciliationEngine*,
                  IAuditEngine*,HealthMonitor*,Watchdog*) noexcept;
    BarFinalizationState ingest_bar(Timeframe,const Bar&);
    bool record_decision(const ShadowDecision&);
    RuntimeState snapshot() const noexcept;
    const ShadowLedger& ledger() const noexcept;
    void ingest_tick(const Tick& tick);
    DecisionFlowResult process_bar(Timeframe,const Bar&bar);
    void set_symbol_spec(const SymbolSpec& spec) noexcept { symbol_spec_=spec; }
    const SymbolSpec& symbol_spec() const noexcept { return symbol_spec_; }
    void set_account_state(const AccountState& account) noexcept { account_=account; }
    const AccountState& account_state() const noexcept { return account_; }
    void set_risk_limits(const RiskLimits& limits) noexcept { limits_=limits; }
    const RiskLimits& risk_limits() const noexcept { return limits_; }
    std::size_t open_position_count() const noexcept { return open_positions_.size(); }
    const std::vector<Position>& closed_positions() const noexcept { return closed_positions_; }
    const std::vector<ReconciliationResult>& closed_reconciliations() const noexcept { return closed_reconciliations_; }
private:
    struct OpenPositionRecord { Position position{}; Signal signal{}; RiskProposal proposal{}; };
    IBarFinalizer* finalizer_{}; ITimeframeStateStore* state_store_{}; ShadowLedger* ledger_{};
    IFeatureEngine* features_{}; IStructureEngine* structure_{}; IRegimeEngine* regime_{}; IEligibilityEngine* eligibility_{}; ISignalEngine* signal_{};
    IScoreEngine* score_{}; IConfidenceEngine* confidence_{}; IMacroContextEngine* macro_{}; IMarketQualityEngine* quality_{}; IRiskEngine* risk_{};
    IPortfolioRiskEngine* portfolio_risk_{}; IShadowExecutionEngine* execution_{}; IPositionSimulator* positions_{}; IReconciliationEngine* reconciliation_{};
    IAuditEngine* audit_{}; HealthMonitor* health_{}; Watchdog* watchdog_{}; RuntimeState state_{}; Tick latest_tick_{}; bool has_tick_{false};
    std::map<int,FeatureSnapshot> last_features_; std::map<int,RegimeSnapshot> last_regimes_;
    std::map<std::array<std::uint8_t,16>,OpenPositionRecord> open_positions_; std::vector<Position> closed_positions_; std::vector<ReconciliationResult> closed_reconciliations_;
    AccountState account_{}; RiskLimits limits_{};
    SymbolSpec symbol_spec_{"Research","Shadow","XAUUSD",2,0.01,0.01,1.0,100.0,0.01,100.0,0.01,0.0,0.0};
    void touch_activity(Timestamp) noexcept;
    void settle_closed_position(OpenPositionRecord& record, Timestamp at);
};
} // namespace xauusd::sovereign
