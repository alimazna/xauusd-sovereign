#pragma once
#include "ServiceState.h"
#include "Timestamp.h"
#include <cstddef>
namespace xauusd::sovereign {
class Watchdog{
public:
 bool observe(Timestamp heartbeat,bool adapter_alive,std::size_t queue_depth,std::size_t max_queue,ServiceState expected,ServiceState actual) noexcept;
 bool healthy() const noexcept{return healthy_;}
 const Timestamp& last_heartbeat() const noexcept{return last_heartbeat_;}
private:
 Timestamp last_heartbeat_{};
 bool healthy_{true};
};
}
