#pragma once
#include <string>
#include <utility>
namespace xauusd::sovereign {
class FeatureKey {
public:
    FeatureKey()=default;
    explicit FeatureKey(std::string v):value_(std::move(v)){}
    const std::string& value() const noexcept{return value_;}
    friend bool operator==(const FeatureKey&,const FeatureKey&)=default;
    friend bool operator!=(const FeatureKey&,const FeatureKey&)=default;
    friend bool operator<(const FeatureKey&a,const FeatureKey&b){return a.value_<b.value_;}
private:
    std::string value_;
};
}
