#pragma once
#include "IScoreEngine.h"
namespace xauusd::sovereign {class ScoreEngine final:public IScoreEngine{public:Score compute(const Signal&,const RegimeSnapshot&,const StructureSnapshot&,const FeatureSnapshot&) const override;};}
