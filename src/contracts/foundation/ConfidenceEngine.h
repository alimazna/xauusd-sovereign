#pragma once
#include "IConfidenceEngine.h"
namespace xauusd::sovereign {class ConfidenceEngine final:public IConfidenceEngine{public:Confidence compute(const Signal&,const RegimeSnapshot&,const StructureSnapshot&,const FeatureSnapshot&) const override;};}
