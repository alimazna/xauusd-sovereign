#pragma once

#ifdef ERROR
#undef ERROR
#endif

namespace xauusd::sovereign
{

enum class ServiceState
{
    UNKNOWN,
    STARTING,
    RUNNING,
    DEGRADED,
    STOPPED,
    FAILED,
    ERROR,
    ONLINE,
    OFFLINE,
    BLOCKED,
    PAUSED
};

}
