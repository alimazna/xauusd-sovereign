#pragma once
#include "PositionSizing.h"
#include "SymbolSpec.h"
namespace xauusd::sovereign {class PositionSizer{public:PositionSizing size(double risk_amount,double stop_distance,const SymbolSpec&) const;};}
