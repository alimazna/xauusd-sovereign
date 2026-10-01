#pragma once
#include "IStructureEngine.h"
#include <deque>
#include <map>
namespace xauusd::sovereign {
class StructureEngine final: public IStructureEngine{
public:
    StructureSnapshot analyze(Timeframe,const Bar&) override;
    void clear();
private:
    mutable std::map<int,std::deque<Bar>> history_;
    struct State{bool has_high=false,has_low=false;double prev_high=0,last_high=0,prev_low=0,last_low=0;StructureType last{StructureType::NONE};bool bullish=false;};
    std::map<int,State> state_;
};
}
