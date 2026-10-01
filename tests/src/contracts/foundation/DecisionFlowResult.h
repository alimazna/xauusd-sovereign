#pragma once
#include "Signal.h"
#include "RiskProposal.h"
#include "Position.h"
#include "SimulatedFill.h"
#include "Score.h"
#include "Confidence.h"
#include "RegimeSnapshot.h"
#include "EligibilityResult.h"
#include "MarketQuality.h"
#include "MacroState.h"
#include "ReconciliationResult.h"
namespace xauusd::sovereign {
struct DecisionFlowResult{
 bool data_valid{false};
 RegimeSnapshot regime{};
 EligibilityResult eligibility{};
 Signal signal{};
 Score score{};
 Confidence confidence{};
 MacroState macro{MacroState::UNKNOWN};
 MarketQuality quality{MarketQuality::UNKNOWN};
 RiskProposal risk{};
 SimulatedFill fill{};
 Position position{};
 ReconciliationResult reconciliation{};
 bool completed{false};
};
}
