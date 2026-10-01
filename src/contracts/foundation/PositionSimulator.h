#pragma once
#include "IPositionSimulator.h"
namespace xauusd::sovereign {
class PositionSimulator final:public IPositionSimulator{
public:
 Position open(const SimulatedFill&) override;
 bool update(Position&,const Tick&) override;
 Position close(Position&,double,const std::string&) override;
private:
 Position close(Position&,double,const std::string&,Timestamp);
};
}
