
#pragma once
#include "Bar.h"
#include "Tick.h"
#include "EntityId.h"
#include "Timeframe.h"
#include "FeatureSnapshot.h"
#include "StructureSnapshot.h"
#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <vector>

using namespace xauusd::sovereign;

inline EntityId test_id(std::uint8_t v){std::array<std::uint8_t,16>b{};b[0]=v;return EntityId{b};}
inline Bar test_bar(std::int64_t t,double close,Timeframe tf=Timeframe::M15){
    double open=close-0.2, high=close+0.8, low=close-0.8;
    return Bar{Timestamp{t-1000},Timestamp{t},open,high,low,close,100,100,tf};
}
inline Tick test_tick(std::int64_t t,double mid=2000.0,double spread=0.2){
    return Tick{Timestamp{t},Timestamp{t+10},mid-spread/2,mid+spread/2,mid,1.0,0};
}
inline FeatureSnapshot numeric_features(double close,double atr,double adx,double slope,double ema_fast,double ema_slow,double rsi,double natr){
    FeatureSnapshot f; f.timeframe=Timeframe::M15; f.computed_at=Timestamp{1000};
    auto add=[&](const char*k,double v){f.features.emplace(FeatureKey{k},FeatureValue{v});};
    add("close",close);add("atr",atr);add("adx",adx);add("ema_slope",slope);add("ema_fast",ema_fast);add("ema_slow",ema_slow);add("rsi",rsi);add("natr",natr);
    return f;
}
inline StructureSnapshot bullish_structure(){
    StructureSnapshot s;s.timeframe=Timeframe::M15;s.computed_at=Timestamp{1000};s.last_swing_type=StructureType::HIGHER_LOW;s.last_swing_high=2010;s.last_swing_low=1990;s.has_valid_structure=true;s.is_bullish=true;return s;
}
