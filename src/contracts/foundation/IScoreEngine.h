#pragma once
#include "FeatureSnapshot.h"
#include "RegimeSnapshot.h"
#include "Score.h"
#include "Signal.h"
#include "StructureSnapshot.h"
namespace xauusd::sovereign {class IScoreEngine{public:virtual~IScoreEngine()=default;virtual Score compute(const Signal&,const RegimeSnapshot&,const StructureSnapshot&,const FeatureSnapshot&) const=0;};}
