#include "ReplayEngine.h"
namespace xauusd::sovereign {
void ReplayEngine::set_data(Timeframe tf,std::vector<Bar> bars){data_[static_cast<int>(tf)]=std::move(bars);}
bool ReplayEngine::load(Timeframe tf){auto it=data_.find(static_cast<int>(tf));if(it==data_.end())return false;active_=it->second;index_=0;return true;}
bool ReplayEngine::has_next() const{return index_<active_.size();}
Bar ReplayEngine::next(){if(!has_next())return Bar{};return active_[index_++];}
void ReplayEngine::reset(){index_=0;}
}
