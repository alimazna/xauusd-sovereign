#pragma once
#include "Position.h"
#include "SimulatedFill.h"
#include "Tick.h"
#include <string>
namespace xauusd::sovereign {class IPositionSimulator{public:virtual~IPositionSimulator()=default;virtual Position open(const SimulatedFill&)=0;virtual bool update(Position&,const Tick&)=0;virtual Position close(Position&,double,const std::string&)=0;};}
