#pragma once
#include "IEligibilityEngine.h"
namespace xauusd::sovereign {class EligibilityEngine final:public IEligibilityEngine{public:EligibilityResult evaluate(const RegimeSnapshot&,const StructureSnapshot&,const FeatureSnapshot&) const override;};}
