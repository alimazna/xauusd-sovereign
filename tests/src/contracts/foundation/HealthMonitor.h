#pragma once
#include "EntityId.h"
#include "ServiceState.h"
#include "HealthSnapshot.h"
#include <vector>
namespace xauusd::sovereign {
class HealthMonitor{
public:
 void record(const HealthSnapshot&);
 bool get(EntityId,HealthSnapshot&) const;
 std::vector<HealthSnapshot> all() const;
 ServiceState overall_state() const noexcept;
 std::size_t size() const noexcept;
 void clear();
private:
 std::vector<HealthSnapshot> snapshots_;
};
}
