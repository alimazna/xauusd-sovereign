#include "Watchdog.h"

namespace xauusd::sovereign
{

bool Watchdog::observe(
    Timestamp heartbeat,
    bool adapter_alive,
    std::size_t queue_depth,
    std::size_t max_queue,
    ServiceState expected,
    ServiceState actual) noexcept
{
    last_heartbeat_ = heartbeat;

    healthy_ =
        adapter_alive &&
        queue_depth <= max_queue &&
        expected == actual &&
        actual != ServiceState::ERROR;

    return healthy_;
}

}