#include "PersistenceEngine.h"
namespace xauusd::sovereign {
bool PersistenceEngine::write(const PersistenceRecordMetadata&m,std::vector<std::uint8_t> p){records_[m.record_id.bytes()]=Entry{m,std::move(p)};return true;}
bool PersistenceEngine::read(EntityId id,PersistenceRecordMetadata&m,std::vector<std::uint8_t>&p) const{auto it=records_.find(id.bytes());if(it==records_.end())return false;m=it->second.metadata;p=it->second.payload;return true;}
bool PersistenceEngine::contains(EntityId id) const{return records_.count(id.bytes())!=0;}
std::size_t PersistenceEngine::count() const{return records_.size();}
void PersistenceEngine::clear(){records_.clear();}
}
