#pragma once
#include "EligibilityResult.h"
#include "FeatureSnapshot.h"
#include "RegimeSnapshot.h"
#include "StructureSnapshot.h"
namespace xauusd::sovereign {class IEligibilityEngine{public:virtual~IEligibilityEngine()=default;virtual EligibilityResult evaluate(const RegimeSnapshot&,const StructureSnapshot&,const FeatureSnapshot&) const=0;};}
