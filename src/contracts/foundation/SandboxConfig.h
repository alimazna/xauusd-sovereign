#pragma once
#include <cstdint>
#include <string>
#include <utility>
namespace xauusd::sovereign {
struct SandboxConfig {
    std::uint64_t max_wall_time_ms{0}; std::uint64_t max_memory_bytes{0}; bool allow_network{false}; bool allow_filesystem{false}; std::string label{};
    SandboxConfig() = default;
    SandboxConfig(std::uint64_t max_wall_time_ms,std::uint64_t max_memory_bytes,bool allow_network,bool allow_filesystem,std::string label)
        : max_wall_time_ms(max_wall_time_ms),max_memory_bytes(max_memory_bytes),allow_network(allow_network),allow_filesystem(allow_filesystem),label(std::move(label)) {}
};
}
