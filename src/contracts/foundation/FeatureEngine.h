#pragma once
#include "IFeatureEngine.h"
#include <deque>
#include <map>
namespace xauusd::sovereign {
class FeatureEngine final: public IFeatureEngine {
public:
    FeatureSnapshot compute(Timeframe,const Bar&,const FeatureSnapshot* previous) const override;
    std::size_t feature_count() const override;
    void clear();
private:
    mutable std::map<int,std::deque<Bar>> history_;
};
}
