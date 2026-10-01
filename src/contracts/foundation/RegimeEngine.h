#pragma once
#include "IRegimeEngine.h"
#include <map>
namespace xauusd::sovereign {
class RegimeEngine final:public IRegimeEngine{
public:
    RegimeSnapshot classify(Timeframe,const FeatureSnapshot&,const StructureSnapshot&,const RegimeSnapshot* previous) override;
    void clear();
private:
    struct Pending{RegimeType candidate{RegimeType::UNKNOWN};std::uint32_t count{0};};
    std::map<int,Pending> pending_;
};
}
