#pragma once
#include "IPersistenceStore.h"
#include "PersistenceRecordMetadata.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <map>
#include <vector>
namespace xauusd::sovereign {
class PersistenceEngine{
public:
 bool write(const PersistenceRecordMetadata&,std::vector<std::uint8_t> payload);
 bool read(EntityId,PersistenceRecordMetadata&,std::vector<std::uint8_t>&) const;
 bool contains(EntityId) const; std::size_t count() const; void clear();
private:
 struct Entry{PersistenceRecordMetadata metadata;std::vector<std::uint8_t> payload;};
 std::map<std::array<std::uint8_t,16>,Entry> records_;
};
}
