
#include "TestHelpers.h"
#include "ReplayEngine.h"
#include "FeatureEngine.h"
#include "StructureEngine.h"
#include "RegimeEngine.h"
#include "EligibilityEngine.h"
#include "SignalEngine.h"
#include "ScoreEngine.h"
#include "ConfidenceEngine.h"
#include <cassert>
#include <cmath>
#include <vector>
using namespace xauusd::sovereign;

struct RunOut {std::vector<int> regimes; std::vector<int> directions; std::vector<double> scores;};

static RunOut run_once(const std::vector<Bar>& bars){
 FeatureEngine fe;StructureEngine se;RegimeEngine re;EligibilityEngine ee;SignalEngine si;ScoreEngine sc;ConfidenceEngine co;
 RunOut o;
 for(const auto& b:bars){
  auto f=fe.compute(b.timeframe,b,nullptr);auto s=se.analyze(b.timeframe,b);auto r=re.classify(b.timeframe,f,s,nullptr);
  auto e=ee.evaluate(r,s,f);auto sg=si.generate(e,r,s,f);auto score=sc.compute(sg,r,s,f);auto conf=co.compute(sg,r,s,f);
  o.regimes.push_back(static_cast<int>(r.regime));o.directions.push_back(static_cast<int>(sg.direction));o.scores.push_back(score.value+conf.numeric/1000.0);
 }
 return o;
}
int main(){
 std::vector<Bar> bars;bars.reserve(40);
 for(int i=0;i<40;++i){double c=2000.0+0.6*i+std::sin(i*0.5)*0.8;bars.push_back(test_bar(1000+i*1000,c));}
 auto a=run_once(bars);auto b=run_once(bars);
 assert(a.regimes==b.regimes);assert(a.directions==b.directions);assert(a.scores.size()==b.scores.size());
 for(std::size_t i=0;i<a.scores.size();++i)assert(std::abs(a.scores[i]-b.scores[i])<1e-12);
 return 0;
}
