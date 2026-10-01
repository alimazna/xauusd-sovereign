#include "ConfigurationRegistry.h"
namespace xauusd::sovereign {
void ConfigurationRegistry::set(ConfigurationKey k,ConfigurationValue v){entries_[std::move(k)]=std::move(v);version_=Version{version_.value()+1};}
bool ConfigurationRegistry::get(const ConfigurationKey k,ConfigurationValue& out) const{auto it=entries_.find(k);if(it==entries_.end())return false;out=it->second;return true;}
ConfigurationSnapshot ConfigurationRegistry::snapshot() const{return ConfigurationSnapshot{version_,ConfigurationScope::RUNTIME,entries_};}
void ConfigurationRegistry::clear(){entries_.clear();version_=Version{version_.value()+1};}
}
