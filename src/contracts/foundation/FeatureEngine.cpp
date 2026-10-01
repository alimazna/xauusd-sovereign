#include "FeatureEngine.h"
#include "EngineIdentity.h"
#include "DataQualityState.h"
#include <algorithm>
#include <cmath>
#include <numeric>
#include <string>
#include <vector>
namespace xauusd::sovereign {
namespace{int tf_key(Timeframe t){return static_cast<int>(t);}double clamp01(double v){return std::max(0.0,std::min(1.0,v));}double true_range(const Bar&a,const Bar&prev){return std::max({a.high-a.low,std::abs(a.high-prev.close),std::abs(a.low-prev.close)});}}
FeatureSnapshot FeatureEngine::compute(Timeframe timeframe,const Bar&bar,const FeatureSnapshot*) const {
 auto&q=history_[tf_key(timeframe)]; FeatureSnapshot out; out.timeframe=timeframe;out.computed_at=bar.close_time;
 out.source_bar_id=detail::make_id("bar",static_cast<std::uint64_t>(bar.close_time.value()),static_cast<std::uint64_t>(timeframe),0,0);
 out.snapshot_id=detail::make_id("features",static_cast<std::uint64_t>(bar.close_time.value()),static_cast<std::uint64_t>(timeframe),q.size(),0);
 const char* names[]={"true_range","atr","natr","returns","return_volatility","adx","ema_fast","ema_slow","ema_slope","rsi","high_low_range","close_location","close"};for(const char*n:names)out.features.emplace(FeatureKey{n},FeatureValue{false});
 if(bar.timeframe!=timeframe){out.data_quality=DataQualityState::INVALID;return out;}
 if(q.empty()){q.push_back(bar);out.data_quality=DataQualityState::VALID;}
 else if(bar.close_time.value()>q.back().close_time.value()){q.push_back(bar);out.data_quality=DataQualityState::VALID;}
 else if(bar.close_time.value()==q.back().close_time.value()){out.data_quality=DataQualityState::DUPLICATE;out.computed_at=q.back().close_time;}
 else {out.data_quality=DataQualityState::OUT_OF_ORDER;out.computed_at=q.back().close_time;}
 if(q.size()>64)q.pop_front();
 out.snapshot_id=detail::make_id("features",static_cast<std::uint64_t>(out.computed_at.value()),static_cast<std::uint64_t>(timeframe),q.size(),static_cast<std::uint64_t>(out.data_quality));
 if(q.size()<14)return out;
 std::vector<double>trs,rets,gains,losses;trs.reserve(14);rets.reserve(14);gains.reserve(14);losses.reserve(14);const std::size_t start=q.size()>15?q.size()-15:0;
 for(std::size_t i=start+1;i<q.size();++i){trs.push_back(true_range(q[i],q[i-1]));const double r=q[i-1].close!=0.0?q[i].close/q[i-1].close-1.0:0.0;rets.push_back(r);const double ch=q[i].close-q[i-1].close;gains.push_back(std::max(0.0,ch));losses.push_back(std::max(0.0,-ch));}
 const double atr=std::accumulate(trs.begin(),trs.end(),0.0)/static_cast<double>(trs.size());const double mean=std::accumulate(rets.begin(),rets.end(),0.0)/static_cast<double>(rets.size());double var=0.0;for(double r:rets)var+=(r-mean)*(r-mean);var/=static_cast<double>(rets.size());const double vol=std::sqrt(std::max(0.0,var));
 double ema_fast=q.front().close,ema_slow=q.front().close,ema_fast_prev=ema_fast;const double af=2.0/10.0,as=2.0/22.0;for(std::size_t i=0;i<q.size();++i){if(i+1==q.size())ema_fast_prev=ema_fast;ema_fast=af*q[i].close+(1.0-af)*ema_fast;ema_slow=as*q[i].close+(1.0-as)*ema_slow;}
 const double prev_close=q.size()>1?q[q.size()-2].close:q.back().close;const double ret=prev_close!=0.0?q.back().close/prev_close-1.0:0.0;
 double tr_sum=0.0,plus=0.0,minus=0.0;const std::size_t dm_start=q.size()>15?q.size()-15:0;for(std::size_t i=dm_start+1;i<q.size();++i){const double up=q[i].high-q[i-1].high,down=q[i-1].low-q[i].low,tr=true_range(q[i],q[i-1]);tr_sum+=tr;if(up>down&&up>0)plus+=up;if(down>up&&down>0)minus+=down;}const double pdi=tr_sum>0?100.0*plus/tr_sum:0.0,mdi=tr_sum>0?100.0*minus/tr_sum:0.0,dx=(pdi+mdi)>0?100.0*std::abs(pdi-mdi)/(pdi+mdi):0.0;
 const double rgain=std::accumulate(gains.begin(),gains.end(),0.0)/static_cast<double>(gains.size()),rloss=std::accumulate(losses.begin(),losses.end(),0.0)/static_cast<double>(losses.size());const double rs=rloss>0?rgain/rloss:100.0,rsi=rloss==0?100.0:100.0-100.0/(1.0+rs);const double hl=q.back().high-q.back().low,loc=hl>0?clamp01((q.back().close-q.back().low)/hl):0.5;
 out.features[FeatureKey{"true_range"}]=FeatureValue{trs.back()};out.features[FeatureKey{"atr"}]=FeatureValue{atr};out.features[FeatureKey{"natr"}]=FeatureValue{q.back().close!=0?100.0*atr/q.back().close:0.0};out.features[FeatureKey{"returns"}]=FeatureValue{ret};out.features[FeatureKey{"return_volatility"}]=FeatureValue{vol};out.features[FeatureKey{"adx"}]=FeatureValue{dx};out.features[FeatureKey{"ema_fast"}]=FeatureValue{ema_fast};out.features[FeatureKey{"ema_slow"}]=FeatureValue{ema_slow};out.features[FeatureKey{"ema_slope"}]=FeatureValue{ema_fast-ema_fast_prev};out.features[FeatureKey{"rsi"}]=FeatureValue{rsi};out.features[FeatureKey{"high_low_range"}]=FeatureValue{hl};out.features[FeatureKey{"close_location"}]=FeatureValue{loc};out.features[FeatureKey{"close"}]=FeatureValue{q.back().close};return out;
}
std::size_t FeatureEngine::feature_count() const{return 13;}void FeatureEngine::clear(){history_.clear();}
}
