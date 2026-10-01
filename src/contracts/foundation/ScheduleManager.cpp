#include "ScheduleManager.h"
#include <algorithm>
namespace xauusd::sovereign {
namespace {
bool valid_window(const ScheduleWindow&w){
 return w.end_time.value()>w.start_time.value()&&w.duration_us>0&&w.minimum_duration_us<=w.duration_us&&w.duration_us<=w.maximum_duration_us&&w.maximum_duration_us>=w.minimum_duration_us;
}
bool inside(const ScheduleWindow&w,Timestamp at){return at.value()>=w.start_time.value()&&at.value()<=w.end_time.value();}
}
bool ScheduleManager::configure(const OperatingSchedule&schedule){
 if(schedule.timezone.empty()||schedule.daily_maximum_us<schedule.daily_minimum_us)return false;
 for(std::size_t i=0;i<schedule.windows.size();++i){if(!valid_window(schedule.windows[i]))return false;for(std::size_t j=i+1;j<schedule.windows.size();++j){if(schedule.windows[j].start_time.value()<schedule.windows[i].end_time.value()&&schedule.windows[i].start_time.value()<schedule.windows[j].end_time.value())return false;}}
 schedule_=schedule;mode_=OperatingMode::SCHEDULED;return true;
}
OperatingMode ScheduleManager::current_mode()const{return mode_;}
bool ScheduleManager::transition_to(OperatingMode new_mode,Timestamp at,const std::string&reason){
 if(reason.empty())return false;
 if(new_mode==mode_)return false;
 const bool safety_transition=(new_mode==OperatingMode::OFFLINE||new_mode==OperatingMode::DRAINING||new_mode==OperatingMode::SAFE_SHUTDOWN||new_mode==OperatingMode::EMERGENCY_STOP);
 if(new_mode==OperatingMode::STARTING||new_mode==OperatingMode::ACTIVE){
   const auto it=std::find_if(schedule_.windows.begin(),schedule_.windows.end(),[&](const auto&w){return inside(w,at)&&w.runtime_enabled&&w.duration_us>=w.minimum_duration_us&&w.duration_us<=w.maximum_duration_us;});
   if(it==schedule_.windows.end())return false;
 } else if(!safety_transition && new_mode!=OperatingMode::SCHEDULED && new_mode!=OperatingMode::PAUSED){
   return false;
 }
 events_.emplace_back(at,mode_,new_mode,reason);mode_=new_mode;return true;
}
std::size_t ScheduleManager::event_count()const{return events_.size();}
std::vector<ScheduleEvent> ScheduleManager::all_events()const{return events_;}
void ScheduleManager::clear(){schedule_=OperatingSchedule{};mode_=OperatingMode::OFFLINE;events_.clear();}
} // namespace xauusd::sovereign
