
#include "TestHelpers.h"
#include "BarFinalizer.h"
#include "DataValidator.h"
#include "RuntimeEngine.h"
#include "TimeframeStateStore.h"
#include "ShadowLedger.h"
#include "AuditEngine.h"
#include "HealthMonitor.h"
#include "Watchdog.h"
#include "FeatureEngine.h"
#include "StructureEngine.h"
#include "RegimeEngine.h"
#include "EligibilityEngine.h"
#include "SignalEngine.h"
#include "ScoreEngine.h"
#include "ConfidenceEngine.h"
#include "MacroContextEngine.h"
#include "MarketQualityEngine.h"
#include "RiskEngine.h"
#include "PortfolioRiskEngine.h"
#include "ShadowExecutionEngine.h"
#include "PositionSimulator.h"
#include "ReconciliationEngine.h"
#include <cassert>
#include <vector>
using namespace xauusd::sovereign;

int main(){
 DataValidator dv; BarFinalizer fin(&dv); TimeframeStateStore store; ShadowLedger ledger;
 FeatureEngine fe; StructureEngine se; RegimeEngine re; EligibilityEngine ee; SignalEngine sige;
 ScoreEngine sce; ConfidenceEngine ce; MacroContextEngine me; MarketQualityEngine mqe;
 RiskEngine rke; PortfolioRiskEngine pre(0.10,100); ShadowExecutionEngine x; PositionSimulator ps; ReconciliationEngine rec;
 AuditEngine audit; HealthMonitor hm; Watchdog wd;
 RuntimeEngine rt(&fin,&store,&ledger,&fe,&se,&re,&ee,&sige,&sce,&ce,&me,&mqe,&rke,&pre,&x,&ps,&rec,&audit,&hm,&wd);
 for(int i=0;i<25;++i){
   const double c=2000.0+0.5*i+((i%5==0)?2.0:0.0);
   rt.ingest_tick(test_tick(10000+i*100,c,.2));
   auto r=rt.process_bar(Timeframe::M15,test_bar(10000+i*100,c));
   assert(r.data_valid);
 }
 assert(rt.snapshot().bars_processed==25);
 assert(rt.snapshot().ticks_processed==25);
 (void)hm.size(); // health is emitted when a complete decision is produced
 return 0;
}
