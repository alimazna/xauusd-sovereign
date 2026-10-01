#include "FailureDetector.h"
#include "Clock.h"
#include "EngineIdentity.h"
namespace xauusd::sovereign {
bool FailureDetector::observe(const FailurePattern&pattern){const auto key=pattern.pattern_id.bytes();const auto it=index_.find(key);if(it!=index_.end()){auto&existing=patterns_[it->second];existing.last_seen=pattern.last_seen;existing.occurrence_count = pattern.occurrence_count>0?pattern.occurrence_count:existing.occurrence_count;return true;}const auto position=patterns_.size();patterns_.push_back(pattern);index_.emplace(key,position);return true;}
std::vector<FailurePattern> FailureDetector::all_patterns()const{return patterns_;}
FailureReport FailureDetector::generate_report()const{return FailureReport(detail::make_id("failure_report",static_cast<std::uint64_t>(patterns_.size()),patterns_.empty()?0ULL:static_cast<std::uint64_t>(patterns_.front().pattern_id.bytes()[0]),patterns_.empty()?0ULL:static_cast<std::uint64_t>(patterns_.back().pattern_id.bytes()[0])),detail::now_timestamp(),patterns_,{});}
std::size_t FailureDetector::pattern_count()const{return patterns_.size();}
void FailureDetector::clear(){patterns_.clear();index_.clear();}
} // namespace xauusd::sovereign
