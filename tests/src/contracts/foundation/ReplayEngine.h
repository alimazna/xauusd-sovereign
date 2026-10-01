#pragma once
#include "IReplayEngine.h"
#include <cstddef>
#include <map>
#include <vector>
namespace xauusd::sovereign {class ReplayEngine final:public IReplayEngine{public:void set_data(Timeframe,std::vector<Bar>);bool load(Timeframe) override;bool has_next() const override;Bar next() override;void reset() override;private:std::map<int,std::vector<Bar>> data_;std::vector<Bar> active_;std::size_t index_{0};};}
