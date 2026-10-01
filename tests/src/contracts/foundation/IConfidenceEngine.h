#pragma once
#include "Confidence.h"
#include "FeatureSnapshot.h"
#include "RegimeSnapshot.h"
#include "Signal.h"
#include "StructureSnapshot.h"
namespace xauusd::sovereign {class IConfidenceEngine{public:virtual~IConfidenceEngine()=default;virtual Confidence compute(const Signal&,const RegimeSnapshot&,const StructureSnapshot&,const FeatureSnapshot&) const=0;};}
