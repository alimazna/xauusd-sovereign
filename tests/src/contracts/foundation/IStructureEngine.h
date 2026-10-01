#pragma once
#include "Bar.h"
#include "StructureSnapshot.h"
#include "Timeframe.h"
namespace xauusd::sovereign {
class IStructureEngine{public:virtual~IStructureEngine()=default;virtual StructureSnapshot analyze(Timeframe,const Bar&)=0;};
}
