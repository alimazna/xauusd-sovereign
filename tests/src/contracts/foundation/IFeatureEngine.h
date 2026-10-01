#pragma once
#include "Bar.h"
#include "FeatureSnapshot.h"
#include "Timeframe.h"
#include <cstddef>
namespace xauusd::sovereign {
class IFeatureEngine {
public:
    virtual ~IFeatureEngine()=default;
    virtual FeatureSnapshot compute(Timeframe,const Bar&,const FeatureSnapshot* previous) const=0;
    virtual std::size_t feature_count() const=0;
};
}
