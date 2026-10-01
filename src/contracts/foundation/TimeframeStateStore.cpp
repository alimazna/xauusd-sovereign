#include "TimeframeStateStore.h"
namespace xauusd::sovereign {
bool TimeframeStateStore::get_state(Timeframe timeframe,TimeframeState&out_state)const{auto it=states_.find(timeframe);if(it==states_.end())return false;out_state=it->second;return true;}
void TimeframeStateStore::put_state(const TimeframeState&state){
 const bool has_bar_timestamp=state.latest_finalized_bar.close_time.value()!=0||state.latest_finalized_bar.open_time.value()!=0;
 if(has_bar_timestamp&&state.latest_finalized_bar.timeframe!=state.timeframe)return;
 auto it=states_.find(state.timeframe);if(it!=states_.end()&&state.last_update_time.value()<it->second.last_update_time.value())return;
 states_[state.timeframe]=state;
}
bool TimeframeStateStore::has_state(Timeframe timeframe)const{return states_.find(timeframe)!=states_.end();}
std::size_t TimeframeStateStore::size()const noexcept{return states_.size();}
void TimeframeStateStore::clear(){states_.clear();}
}
