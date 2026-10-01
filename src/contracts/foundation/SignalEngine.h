#pragma once
#include "ISignalEngine.h"
namespace xauusd::sovereign {class SignalEngine final:public ISignalEngine{public:Signal generate(const EligibilityResult&,const RegimeSnapshot&,const StructureSnapshot&,const FeatureSnapshot&) override;};}
