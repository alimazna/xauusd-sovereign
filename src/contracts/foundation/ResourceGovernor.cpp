#include "ResourceGovernor.h"
#include <limits>
namespace xauusd::sovereign {
namespace {bool addition_within(std::uint64_t used,std::uint64_t amount,std::uint64_t limit){if(used>limit)return false;return amount<=(limit-used);}}
void ResourceGovernor::set_budget(const ResourceBudget&budget){budget_=budget;usage_=ResourceUsage{};}
bool ResourceGovernor::consume(ResourceType type,std::uint64_t amount){const auto limit_it=budget_.limits.find(type);if(limit_it==budget_.limits.end())return false;auto used_it=usage_.used.find(type);const std::uint64_t used=used_it==usage_.used.end()?0U:used_it->second;if(!addition_within(used,amount,limit_it->second))return false;usage_.used[type]=used+amount;return true;}
bool ResourceGovernor::consume_runtime(std::uint64_t hours){if(!addition_within(usage_.runtime_hours_used,hours,budget_.max_runtime_hours))return false;usage_.runtime_hours_used+=hours;return true;}
bool ResourceGovernor::consume_experiment(){if(!addition_within(usage_.experiments_used,1U,budget_.max_experiments))return false;++usage_.experiments_used;return true;}
bool ResourceGovernor::consume_trial(){if(!addition_within(usage_.trials_used,1U,budget_.max_trials))return false;++usage_.trials_used;return true;}
ResourceUsage ResourceGovernor::current_usage()const{return usage_;}
bool ResourceGovernor::within_budget()const{if(usage_.runtime_hours_used>budget_.max_runtime_hours||usage_.experiments_used>budget_.max_experiments||usage_.trials_used>budget_.max_trials)return false;for(const auto&[type,used]:usage_.used){const auto it=budget_.limits.find(type);if(it==budget_.limits.end()||used>it->second)return false;}return true;}
void ResourceGovernor::clear(){budget_=ResourceBudget{};usage_=ResourceUsage{};}
} // namespace xauusd::sovereign
