#pragma once

#include "Timestamp.h"
#include "SystemHealthLevel.h"

#include <cstdint>
#include <string>
#include <utility>

namespace xauusd::sovereign {

class SystemHealthSnapshot {
public:
    Timestamp          taken_at;
    SystemHealthLevel  level;
    std::uint32_t      healthy_services;
    std::uint32_t      degraded_services;
    std::uint32_t      failed_services;
    std::string        summary;

    SystemHealthSnapshot() = default;

    SystemHealthSnapshot(
        Timestamp taken_at,
        SystemHealthLevel level,
        std::uint32_t healthy_services,
        std::uint32_t degraded_services,
        std::uint32_t failed_services,
        std::string summary)
        : taken_at(taken_at),
          level(level),
          healthy_services(healthy_services),
          degraded_services(degraded_services),
          failed_services(failed_services),
          summary(std::move(summary)) {}
};

} // namespace xauusd::sovereign
