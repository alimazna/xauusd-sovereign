#pragma once
#include "EligibilityResult.h"
#include "FeatureSnapshot.h"
#include "RegimeSnapshot.h"
#include "Signal.h"
#include "StructureSnapshot.h"
namespace xauusd::sovereign {class ISignalEngine{public:virtual~ISignalEngine()=default;virtual Signal generate(const EligibilityResult&,const RegimeSnapshot&,const StructureSnapshot&,const FeatureSnapshot&)=0;};}
