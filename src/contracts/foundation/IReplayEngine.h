#pragma once
#include "Bar.h"
#include "Timeframe.h"
namespace xauusd::sovereign {class IReplayEngine{public:virtual~IReplayEngine()=default;virtual bool load(Timeframe)=0;virtual bool has_next() const=0;virtual Bar next()=0;virtual void reset()=0;};}
