#pragma once

#include <array>
#include <cstdint>
#include <ctime>
#include <mutex>
#include <string>
#include <vector>

namespace xauusd::mt5 {

struct AdapterStatus {
    std::string timeframe;
    bool connected{false};
    std::time_t last_heartbeat{0};
    std::time_t last_activity{0};
    std::uint64_t ticks_received{0};
    std::uint64_t bars_received{0};
};

class MT5AdapterManager {
public:
    MT5AdapterManager();

    void register_adapter(const std::string& timeframe);
    void unregister_adapter(const std::string& timeframe);
    void record_tick(const std::string& timeframe);
    void record_bar(const std::string& timeframe);
    void record_heartbeat(const std::string& timeframe);

    bool is_connected(const std::string& timeframe) const;
    bool all_connected() const;
    AdapterStatus status(const std::string& timeframe) const;
    std::vector<AdapterStatus> all_status() const;

private:
    struct Entry { AdapterStatus status; };
    static constexpr std::array<const char*, 9> kTimeframes{
        "M1", "M5", "M15", "M30", "H1", "H4", "D1", "W1", "MN1"};

    Entry* find(const std::string& timeframe);
    const Entry* find(const std::string& timeframe) const;

    mutable std::mutex mutex_;
    std::array<Entry, 9> adapters_{};
};

} // namespace xauusd::mt5
