#pragma once
#include "ConfigurationKey.h"
#include "ConfigurationValue.h"
#include "ConfigurationSnapshot.h"
#include "Version.h"
#include <map>
namespace xauusd::sovereign {
class ConfigurationRegistry{
public:
 void set(ConfigurationKey key,ConfigurationValue value);
 bool get(const ConfigurationKey key,ConfigurationValue& out) const;
 ConfigurationSnapshot snapshot() const;
 void clear();
private:
 std::map<ConfigurationKey,ConfigurationValue> entries_;
 Version version_{0};
};
}
