#include "StructureEngine.h"
#include "EngineIdentity.h"
#include "DataQualityState.h"
#include <algorithm>
namespace xauusd::sovereign {
namespace{int k(Timeframe t){return static_cast<int>(t);}}
StructureSnapshot StructureEngine::analyze(Timeframe tf,const Bar&bar){
 auto&q=history_[k(tf)];auto& st=state_[k(tf)];StructureSnapshot out;out.timeframe=tf;out.computed_at=bar.close_time;out.snapshot_id=detail::make_id("structure",static_cast<std::uint64_t>(bar.close_time.value()),static_cast<std::uint64_t>(tf),q.size(),0);
 if(bar.timeframe!=tf){out.data_quality=DataQualityState::INVALID;return out;}
 if(q.empty()){q.push_back(bar);out.data_quality=DataQualityState::VALID;}else if(bar.close_time.value()>q.back().close_time.value()){q.push_back(bar);out.data_quality=DataQualityState::VALID;}else if(bar.close_time.value()==q.back().close_time.value()){out.data_quality=DataQualityState::DUPLICATE;out.computed_at=q.back().close_time;}else{out.data_quality=DataQualityState::OUT_OF_ORDER;out.computed_at=q.back().close_time;}
 if(q.size()>64)q.pop_front();out.snapshot_id=detail::make_id("structure",static_cast<std::uint64_t>(out.computed_at.value()),static_cast<std::uint64_t>(tf),q.size(),static_cast<std::uint64_t>(out.data_quality));
 if(q.size()>=5){const std::size_t c=q.size()-3;const Bar&b=q[c];const bool sh=b.high>q[c-1].high&&b.high>=q[c-2].high&&b.high>q[c+1].high&&b.high>=q[c+2].high,sl=b.low<q[c-1].low&&b.low<=q[c-2].low&&b.low<q[c+1].low&&b.low<=q[c+2].low;if(sh){st.prev_high=st.last_high;st.last_high=b.high;st.has_high=true;st.last=(st.prev_high>0&&st.last_high>st.prev_high)?StructureType::HIGHER_HIGH:StructureType::LOWER_HIGH;}if(sl){st.prev_low=st.last_low;st.last_low=b.low;st.has_low=true;st.last=(st.prev_low>0&&st.last_low>st.prev_low)?StructureType::HIGHER_LOW:StructureType::LOWER_LOW;}if(st.has_high&&bar.close>st.last_high)st.last=StructureType::BREAK_OF_STRUCTURE_UP;if(st.has_low&&bar.close<st.last_low)st.last=StructureType::BREAK_OF_STRUCTURE_DOWN;}
 if(q.size()>=20){auto hi=std::max_element(q.end()-20,q.end(),[](const Bar&a,const Bar&b){return a.high<b.high;});auto lo=std::min_element(q.end()-20,q.end(),[](const Bar&a,const Bar&b){return a.low<b.low;});out.channel_upper=hi->high;out.channel_lower=lo->low;const double width=out.channel_upper-out.channel_lower;out.has_valid_structure=width>0.0;if(out.has_valid_structure&&(bar.close<=out.channel_lower+0.2*width||bar.close>=out.channel_upper-0.2*width)){if(st.last==StructureType::NONE)st.last=StructureType::RANGE;}}else out.has_valid_structure=st.has_high&&st.has_low;
 if(st.has_high&&st.has_low){st.bullish=(st.last==StructureType::HIGHER_HIGH||st.last==StructureType::HIGHER_LOW||st.last==StructureType::BREAK_OF_STRUCTURE_UP);if(st.last==StructureType::LOWER_HIGH||st.last==StructureType::LOWER_LOW||st.last==StructureType::BREAK_OF_STRUCTURE_DOWN)st.bullish=false;}
 out.last_swing_high=st.last_high;out.last_swing_low=st.last_low;out.last_swing_type=st.last;out.is_bullish=st.bullish;return out;
}
void StructureEngine::clear(){history_.clear();state_.clear();}
}
