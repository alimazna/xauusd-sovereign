#include "TestHelpers.h"

#include "PersistenceEngine.h"
#include "PersistenceRecordMetadata.h"
int main(){PersistenceEngine p;PersistenceRecordMetadata m{};m.record_id=test_id(9);std::vector<std::uint8_t>x{1,2,3};assert(p.write(m,x));assert(p.contains(m.record_id));std::vector<std::uint8_t>y;PersistenceRecordMetadata n{};assert(p.read(m.record_id,n,y));assert(y==x);return 0;}
