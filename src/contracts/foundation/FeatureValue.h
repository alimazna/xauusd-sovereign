#pragma once
#include <cstdint>
#include <string>
#include <utility>
#include <variant>
namespace xauusd::sovereign {
class FeatureValue {
public:
    using Storage=std::variant<bool,std::int64_t,double,std::string>;
    FeatureValue()=default;
    explicit FeatureValue(Storage v):storage_(std::move(v)){}
    const Storage& storage() const noexcept{return storage_;}
    friend bool operator==(const FeatureValue&,const FeatureValue&)=default;
    friend bool operator!=(const FeatureValue&,const FeatureValue&)=default;
private:
    Storage storage_{false};
};
}
