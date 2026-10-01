#pragma once
#include "FeatureSnapshot.h"
#include "RegimeSnapshot.h"
#include "StructureSnapshot.h"
#include "Timeframe.h"
namespace xauusd::sovereign {class IRegimeEngine{public:virtual~IRegimeEngine()=default;virtual RegimeSnapshot classify(Timeframe,const FeatureSnapshot&,const StructureSnapshot&,const RegimeSnapshot* previous)=0;};}
